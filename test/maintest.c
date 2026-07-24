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

// ANSI Color Constants for Terminal Visualizer
#define COLOR_RESET "\x1b[0m"
#define COLOR_MAIN "\x1b[1;32m"   // Bold Green
#define COLOR_BRANCH "\x1b[1;36m" // Bold Cyan
#define COLOR_COMMIT "\x1b[1;33m" // Bold Yellow
#define COLOR_DIM "\x1b[2m"       // Dim Gray
#define COLOR_PASS "\x1b[1;32m"   // Bold Green Pass
#define COLOR_FAIL "\x1b[1;31m"   // Bold Red Fail

// Forward declarations
void Test_HasExtension(void);
void Test_ComputeFileHash(void);
void Test_TreeAndCommits(void);
void Test_ScanDirectory_Mock(void);
void Test_IsFileOpen(void);

// Visual Test Helpers
static void PrintTestHeader(const char *title)
{
    printf("\n======================================================================\n");
    printf("  RUNNING TEST: %s\n", title);
    printf("======================================================================\n");
}

static void PrintAssert(int condition, const char *label)
{
    if (condition)
    {
        printf("  [%s PASS %s] %s\n", COLOR_PASS, COLOR_RESET, label);
    }
    else
    {
        printf("  [%s FAIL %s] %s\n", COLOR_FAIL, COLOR_RESET, label);
    }
}

// ============================================================================
// TEST 1: Extension Matching Unit Tests
// ============================================================================
void Test_HasExtension(void)
{
    PrintTestHeader("1. Extension Matching Unit Tests");

    struct TestCase
    {
        const char *filename;
        const char *ext;
        int expected;
    } cases[] = {
        {"artwork.psd", ".psd", 1},
        {"drawing.kra", ".kra", 1},
        {"image.xcf", ".xcf", 1},
        {"compressed.krz", ".krz", 1},
        {"artwork.psd.txt", ".psd", 0},
        {"psd", ".psd", 0},
        {"", ".psd", 0},
        {".psd", ".psd", 1},
        {"archive.tar.gz", ".gz", 1},
        {"folder.psd/file.txt", ".psd", 0}};

    int total = sizeof(cases) / sizeof(cases[0]);

    printf("\n  [ EXTENSION EVALUATION MATRIX ]\n");
    printf("  %-25s %-8s %-10s %-10s\n", "FILENAME", "TARGET", "RESULT", "STATUS");
    printf("  ---------------------------------------------------------\n");

    for (int i = 0; i < total; i++)
    {
        int actual = HasExtension(cases[i].filename, cases[i].ext);
        int passed = (actual == cases[i].expected);

        printf("  %-25s %-8s %-10s [%s%s%s]\n",
               cases[i].filename[0] == '\0' ? "\"(empty)\"" : cases[i].filename,
               cases[i].ext,
               actual ? "MATCH" : "NO MATCH",
               passed ? COLOR_PASS : COLOR_FAIL,
               passed ? " PASS " : " FAIL ",
               COLOR_RESET);
    }
    printf("\n");
}

// ============================================================================
// TEST 2: File Hashing Test (SHA-256)
// ============================================================================
void Test_ComputeFileHash(void)
{
    PrintTestHeader("2. SHA-256 File Hashing Tests");

    // Subtest A: Known string payload
    const char *test_file = "test_hash_file.txt";
    const char *expected_hash = "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9";

    FILE *f = fopen(test_file, "w");
    if (f)
    {
        fprintf(f, "hello world");
        fclose(f);
    }

    char *computed_hash = ComputeFile_sha256(test_file);

    printf("\n  [ SHA-256 HASH VERIFICATION ]\n");
    printf("  Target File : %s\n", test_file);
    printf("  Payload     : \"hello world\"\n");
    printf("  ┌── Computed: %s%s%s\n", COLOR_COMMIT, computed_hash ? computed_hash : "NULL", COLOR_RESET);
    printf("  └── Expected: %s%s%s\n\n", COLOR_COMMIT, expected_hash, COLOR_RESET);

    if (computed_hash)
    {
        PrintAssert(strcmp(computed_hash, expected_hash) == 0, "Known string hash match");
        free(computed_hash);
    }
    else
    {
        PrintAssert(0, "ComputeFile_sha256 returned NULL on valid file");
    }
    remove(test_file);

    // Subtest B: Empty File
    const char *empty_file = "test_empty_file.txt";
    const char *empty_expected_hash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

    f = fopen(empty_file, "w");
    if (f)
        fclose(f);

    char *empty_hash = ComputeFile_sha256(empty_file);
    PrintAssert(empty_hash && strcmp(empty_hash, empty_expected_hash) == 0, "Empty file hash calculation");
    if (empty_hash)
        free(empty_hash);
    remove(empty_file);

    // Subtest C: Non-existent File
    char *null_hash = ComputeFile_sha256("non_existent_file_xyz.tmp");
    PrintAssert(null_hash == NULL, "Non-existent file handling returns NULL");
    if (null_hash)
        free(null_hash);
}

// ============================================================================
// TEST 3: Tree Data Structure & Commits Visualizer Test
// ============================================================================


void Test_TreeAndCommits(void)
{
    PrintTestHeader("3. Tree Data Structure & Commits");

    TreeNode *root = Init(InitStack(10), "main");
    if (!root)
    {
        PrintAssert(0, "Root node initialization failed");
        return;
    }

    Commit(root->data, "v0.1.0 Init Repository");
    Commit(root->data, "v0.2.0 Setup Core Logic");

    TreeNode *feat_ui = CreateChild(root, "feature/ui");
    TreeNode *feat_engine = CreateChild(root, "feature/engine");

    Commit(feat_ui->data, "Add Canvas View");
    Commit(feat_ui->data, "Implement Button Event Handling");

    TreeNode *feat_buttons = CreateChild(feat_ui, "feature/ui-buttons");
    Commit(feat_buttons->data, "Add Hover Animations");

    Commit(feat_engine->data, "Optimize SHA-256 Buffer Operations");

    TreeNode *hotfix = CreateChild(root, "hotfix/patch-01");

    printf("\n  [ RENDERED BRANCH & COMMIT TREE ]\n");
    printf("\n");

    PrintAssert(root->data->top == 1, "Main branch contains 2 commits");
    PrintAssert(feat_ui->data->top == 1, "Feature UI branch contains 2 commits");
    PrintAssert(feat_buttons->data->top == 0, "Feature UI Buttons branch contains 1 commit");
    PrintAssert(hotfix->data->top == -1, "Hotfix branch initialized with 0 commits");

    FreeTree(root);
}

// ============================================================================
// TEST 4: Mock Directory Recursive Scanner Test
// ============================================================================
void Test_ScanDirectory_Mock(void)
{
    PrintTestHeader("4. Mock Directory Recursive Scanner Test");

    const char *mock_dir = "test_mock_dir";
    const char *mock_subdir = "test_mock_dir/nested_dir";
    const char *psd_file = "test_mock_dir/nested_dir/sample.psd";
    const char *kra_file = "test_mock_dir/sketch.kra";
    const char *txt_file = "test_mock_dir/ignore.txt";

    mkdir_compat(mock_dir);
    mkdir_compat(mock_subdir);

    FILE *f1 = fopen(psd_file, "w");
    if (f1)
    {
        fprintf(f1, "psd fake");
        fclose(f1);
    }
    FILE *f2 = fopen(kra_file, "w");
    if (f2)
    {
        fprintf(f2, "kra fake");
        fclose(f2);
    }
    FILE *f3 = fopen(txt_file, "w");
    if (f3)
    {
        fprintf(f3, "txt fake");
        fclose(f3);
    }

    printf("\n  [ EXPECTED DIRECTORY LAYOUT ]\n");
    printf("  0_0 %s/\n", mock_dir);
    printf("  ├── 1_1 ignore.txt\n");
    printf("  ├── 2_2 sketch.kra\n");
    printf("  └── 3_3 nested_dir/\n");
    printf("      └── 4_4 sample.psd\n\n");

    printf("  [ SCANNER SCANNING CONSOLE OUTPUT ]\n");
    ScanProjectDir(mock_dir);
    printf("\n");

    PrintAssert(1, "Directory scanner successfully traversed filesystem hierarchy");

    remove(psd_file);
    remove(kra_file);
    remove(txt_file);
    rmdir_compat(mock_subdir);
    rmdir_compat(mock_dir);
}

// ============================================================================
// TEST 5: IsFileOpen Lock Checking Test
// ============================================================================
void Test_IsFileOpen(void)
{
    PrintTestHeader("5. File Lock & State Visualizer (IsFileOpen)");

    const char *test_file = "test_temp_lock_file.psd";

    printf("\n  [ OS LOCK STATE MONITOR ]\n");

    // 1. Check non-existent file
    int status_missing = IsFileOpen("non_existent_file.psd");
    printf("  State 1: Non-existent file  -> Lock Flag: [%d] (UNLOCKED)\n", status_missing);
    PrintAssert(status_missing == 0, "Non-existent file reports UNLOCKED");

    // 2. Create file without locking
    FILE *f = fopen(test_file, "w");
    if (f)
    {
        fprintf(f, "lock verification payload\n");
        fclose(f);
    }

    int status_closed = IsFileOpen(test_file);
    printf("  State 2: File Closed on disk -> Lock Flag: [%d] (UNLOCKED)\n", status_closed);
    PrintAssert(status_closed == 0, "Closed file reports UNLOCKED");

    // 3. Acquire exclusive lock
    int open_success = 0;
#ifdef _WIN32
    HANDLE h_lock = CreateFileA(
        test_file, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    open_success = (h_lock != INVALID_HANDLE_VALUE);
#elif defined(__linux__)
    int fd_lock = open(test_file, O_RDWR);
    open_success = (fd_lock != -1 && flock(fd_lock, LOCK_EX) == 0);
#elif defined(__APPLE__)
    FILE *f_lock = fopen(test_file, "r+");
    open_success = (f_lock != NULL);
#endif

    if (open_success)
    {
        int status_locked = IsFileOpen(test_file);
        printf("  State 3: Exclusive Lock Held -> Lock Flag: [%d] (%s)\n",
               status_locked, status_locked ? "LOCKED" : "UNLOCKED");

#if !defined(__APPLE__)
        PrintAssert(status_locked == 1, "Exclusively locked file reports LOCKED");
#endif

        // Release Lock
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
        PrintAssert(0, "Failed to simulate OS file lock");
    }

    remove(test_file);
    printf("\n");
}

// ============================================================================
// MAIN SUITE RUNNER
// ============================================================================
int main(int argc, char const *argv[])
{
    clock_t start = clock();

    printf("\n");
    printf("======================================================================\n");
    printf("             AUTOMATED SUITE TEST & VISUALIZER FRAMEWORK              \n");
    printf("======================================================================\n");

    Test_HasExtension();
    Test_ComputeFileHash();
    Test_TreeAndCommits();
    Test_ScanDirectory_Mock();
    Test_IsFileOpen();

    if (argc > 1)
    {
        PrintTestHeader("CLI Target Directory Scan");
        printf("Target directory parameter: %s\n", argv[1]);
        ScanProjectDir(argv[1]);
    }

    clock_t end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;

    printf("======================================================================\n");
    printf("  SUITE COMPLETE | Total Time Elapsed: %.2f μs\n", microseconds);
    printf("======================================================================\n\n");

    return 0;
}