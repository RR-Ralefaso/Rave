#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir_compat(path) _mkdir(path)
#define rmdir_compat(path) _rmdir(path)
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#define mkdir_compat(path) mkdir(path, 0755)
#define rmdir_compat(path) rmdir(path)
#endif

#include "../src/structures/structures.h"
#include "../src/structures/structure_traversal/StructureTraversal.h"
#include "../src/branch/branches.h"
#include "../src/commits/commits.h"
#include "../src/branch/merge/merge.h"
#include "../src/storage/hash/hash.h"
#include "../src/checker/checks.h"

// Forward declarations of test functions
void Test_HasExtension(void);
void Test_ComputeFileHash(void);
void Test_TreeAndCommits(void);
void Test_ScanDirectory_Mock(void);
void Test_IsFileOpen(void);

// ============================================================================
// TEST 1: Extension Matching Unit Tests
// ============================================================================
void Test_HasExtension(void)
{
    printf("--- RUNNING HASEXTENSION TEST ---\n");

    int pass = 1;

    if (!HasExtension("artwork.psd", ".psd"))
        pass = 0;
    if (!HasExtension("drawing.kra", ".kra"))
        pass = 0;
    if (!HasExtension("image.xcf", ".xcf"))
        pass = 0;
    if (!HasExtension("compressed.krz", ".krz"))
        pass = 0;

    // False positives / Edge cases
    if (HasExtension("artwork.psd.txt", ".psd"))
        pass = 0;
    if (HasExtension("psd", ".psd"))
        pass = 0;
    if (HasExtension("", ".psd"))
        pass = 0;

    if (pass)
    {
        printf("[TEST PASS] HasExtension correctly identifies extension edge cases.\n\n");
    }
    else
    {
        printf("[TEST FAIL] HasExtension failed on one or more extension checks.\n\n");
    }
}

// ============================================================================
// TEST 2: File Hashing Test (SHA-256)
// ============================================================================
void Test_ComputeFileHash(void)
{
    printf("--- RUNNING HASHING TEST ---\n");

    const char *test_file = "test_hash_file.txt";

    // Known SHA-256 string for "hello world"
    const char *expected_hash = "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9";

    FILE *f = fopen(test_file, "w");
    if (!f)
    {
        printf("[TEST FAIL] Could not create temp file for hashing.\n\n");
        return;
    }
    fprintf(f, "hello world");
    fclose(f);

    char *computed_hash = ComputeFile_sha256(test_file);
    if (computed_hash != NULL)
    {
        if (strcmp(computed_hash, expected_hash) == 0)
        {
            printf("[TEST PASS] File hash matches expected SHA-256 string!\n");
            printf("            Hash: %s\n", computed_hash);
        }
        else
        {
            printf("[TEST FAIL] Hash mismatch!\n");
            printf("            Got:      %s\n", computed_hash);
            printf("            Expected: %s\n", expected_hash);
        }
        free(computed_hash);
    }
    else
    {
        printf("[TEST FAIL] ComputeFile_sha256 returned NULL.\n");
    }

    remove(test_file);
    printf("--- HASH TEST COMPLETED ---\n\n");
}

// ============================================================================
// TEST 3: Tree Data Structure & Commits Test
// ============================================================================
void Test_TreeAndCommits(void)
{
    printf("--- RUNNING TREE & COMMIT TEST ---\n");

    TreeNode *root = Init(InitStack(1), "master");
    if (!root)
    {
        printf("[TEST FAIL] Root node initialization failed.\n\n");
        return;
    }

    TreeNode *child = CreateChild(root, "feature_branch");
    if (!child)
    {
        printf("[TEST FAIL] Child node creation failed.\n");
        FreeTree(root);
        return;
    }

    char commit1_data[] = "Initial commit";
    int commit2_data = 100;

    Commit(root->data, commit1_data);
    Commit(child->data, &commit2_data);

    if (root->data->top >= 0 && child->data->top >= 0)
    {
        printf("[TEST PASS] Tree creation and commit pushes succeeded.\n");
        printf("            Master branch payload: %s\n", (char *)root->data->arr[root->data->top].data);
    }
    else
    {
        printf("[TEST FAIL] Stack top index invalid after commits.\n");
    }

    FreeTree(root);
    printf("--- TREE & COMMIT TEST COMPLETED ---\n\n");
}

// ============================================================================
// TEST 4: Mock Directory Recursive Scanner Test
// ============================================================================
void Test_ScanDirectory_Mock(void)
{
    printf("--- RUNNING MOCK DIRECTORY SCANNER TEST ---\n");

    const char *mock_dir = "test_mock_dir";
    const char *mock_subdir = "test_mock_dir/subdir";
    const char *psd_file = "test_mock_dir/subdir/sample.psd";
    const char *txt_file = "test_mock_dir/ignore.txt";

    mkdir_compat(mock_dir);
    mkdir_compat(mock_subdir);

    FILE *f1 = fopen(psd_file, "w");
    if (f1)
    {
        fprintf(f1, "psd fake");
        fclose(f1);
    }

    FILE *f2 = fopen(txt_file, "w");
    if (f2)
    {
        fprintf(f2, "txt fake");
        fclose(f2);
    }

    printf("Scanning generated mock folder...\n");
    ScanProjectDir(mock_dir);

    // Clean up mock directory files
    remove(psd_file);
    remove(txt_file);
    rmdir_compat(mock_subdir);
    rmdir_compat(mock_dir);

    printf("--- MOCK DIRECTORY SCANNER TEST COMPLETED ---\n\n");
}

// ============================================================================
// TEST 5: IsFileOpen Lock Checking Test
// ============================================================================
void Test_IsFileOpen(void)
{
    const char *test_file = "test_temp_lock_file.psd";

    printf("--- RUNNING ISFILEOPEN TEST ---\n");

    // 1. Create a dummy test file
    FILE *f = fopen(test_file, "w");
    if (!f)
    {
        printf("[TEST FAIL] Could not create dummy test file.\n\n");
        return;
    }
    fprintf(f, "test payload\n");
    fclose(f);

    // 2. Check status when file is closed
    int status_closed = IsFileOpen(test_file);
    if (status_closed == 0)
    {
        printf("[TEST PASS] File correctly reported as CLOSED (0).\n");
    }
    else
    {
        printf("[TEST FAIL] File reported as OPEN (%d) when it should be closed.\n", status_closed);
    }

    // 3. Lock/open the file exclusively according to platform
#ifdef _WIN32
    HANDLE h_lock = CreateFileA(
        test_file,
        GENERIC_READ | GENERIC_WRITE,
        0, // Exclusive access lock
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    int open_success = (h_lock != INVALID_HANDLE_VALUE);
#elif defined(__linux__)
    int fd_lock = open(test_file, O_RDWR);
    int open_success = (fd_lock != -1 && flock(fd_lock, LOCK_EX) == 0);
#elif defined(__APPLE__)
    FILE *f_lock = fopen(test_file, "r+");
    int open_success = (f_lock != NULL);
#else
    int open_success = 0;
#endif

    if (open_success)
    {
        // 4. Test if IsFileOpen detects lock
        int status_open = IsFileOpen(test_file);

#if defined(__APPLE__)
        printf("[TEST INFO] macOS checks other PIDs; current process file check returned: %d\n", status_open);
#else
        if (status_open == 1)
        {
            printf("[TEST PASS] File correctly reported as OPEN/LOCKED (1).\n");
        }
        else
        {
            printf("[TEST FAIL] File reported as CLOSED (%d) when it should be locked.\n", status_open);
        }
#endif

        // 5. Release lock
#ifdef _WIN32
        CloseHandle(h_lock);
#elif defined(__linux__)
        flock(fd_lock, LOCK_UN);
        close(fd_lock);
#elif defined(__APPLE__)
        fclose(f_lock);
#endif
    }
    else
    {
        printf("[TEST FAIL] Failed to simulate locked state on dummy file.\n");
    }

    // 6. Clean up dummy file
    remove(test_file);
    printf("--- ISFILEOPEN TEST COMPLETED ---\n\n");
}

// ============================================================================
// MAIN SUITE RUNNER
// ============================================================================
int main(int argc, char const *argv[])
{
    clock_t start, end;
    start = clock();

    printf("============ RUNNING TEST SUITE ============\n\n");

    Test_HasExtension();
    Test_ComputeFileHash();
    Test_TreeAndCommits();
    Test_ScanDirectory_Mock();
    Test_IsFileOpen();

    printf("============ SUITE TESTS COMPLETE ============\n\n");

    // Optional directory scan from command line argument
    if (argc > 1)
    {
        printf("Target scanning passed directory: %s\n", argv[1]);
        ScanProjectDir(argv[1]);
    }

    end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}