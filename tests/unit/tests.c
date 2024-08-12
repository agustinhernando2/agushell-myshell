#include <unity.h>
#include <functions.h>

void test_test(){
    TEST_ASSERT_EQUAL(0,0);
}

void test_generate_prompt(){
    char* username = "username";
    char* hostname = "hostname";
    char* prompt = generate_prompt(username, hostname);

    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    char buffer[BUFFER_SIZE];
    int r = snprintf(buffer, sizeof(buffer), YELLOW "%s@" GRAY "%s:~%s$" BLUE ">", username, hostname, pwd);
    if (r < 0 || r >= sizeof(buffer))
    {
        TEST_ASSERT_EQUAL_STRING(NULL, prompt);
    }
    TEST_ASSERT_EQUAL_STRING(buffer, prompt);
}

void test_msg_null(void)
{
    char buffer[BUFFER_SIZE];
    int ret = echo_shell(NULL, 0, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(-3, ret);
}

void test_bufflen_zero(void)
{
    char buffer[BUFFER_SIZE];
    int ret = echo_shell("Hola", 4, buffer, 0);
    TEST_ASSERT_EQUAL_INT(-4, ret);
}

void test_env_var_exists(void)
{
    char buffer[BUFFER_SIZE];
    int ret = echo_shell("$HOME", 5, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING(getenv("HOME"), buffer);
}

void test_env_var_not_exists(void)
{
    char buffer[BUFFER_SIZE];
    int ret = echo_shell("$NO_EXISTE", 10, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_STRING(RED "Environment not found: NO_EXISTE" NORMAL, buffer);
}

void test_env_var_not_exists_small_buffer(void)
{
    char buffer[5];
    int ret = echo_shell("$NO_EXISTE", 10, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_INT(-2, ret);
}

void test_valid_comment(void)
{
    char buffer[BUFFER_SIZE];
    char* comment = "\"Este es un comentario\"";
    int ret = echo_shell(comment, strlen(comment), buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_CHAR(comment[0], '"');
    TEST_ASSERT_EQUAL_CHAR(comment[strlen(comment) - 1], '"');
    TEST_ASSERT_EQUAL_STRING("comment: msglen[23], commlen[21], Este es un comentario", buffer);
}

void test_normal_message(void)
{
    char buffer[BUFFER_SIZE];
    int ret = echo_shell("Hola Mundo", 10, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("Hola Mundo", buffer);
}

void test_exchange_directory(void)
{
    setenv("PWD", "/", 1);
    char* command = "/";
    int ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("/", getenv("PWD"));
}

void test_exchange_directory_fail(void)
{
    setenv("PWD", "/", 1);
    char* command = "/noexiste";
    int ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_STRING("/", getenv("PWD"));
}

void test_exchange_directory_pass(void)
{
    setenv("PWD", "/", 1);
    char* command = "home";
    int ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("/home", getenv("PWD"));
}

void test_exchange_directory_pass2(void)
{
    setenv("PWD", "/home", 1);
    char* command = "/dev";
    int ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("/dev", getenv("PWD"));
}

void test_exchange_directory_pass3(void)
{
    setenv("PWD", "/", 1);
    char* command = "/home";
    int ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);

    command = "/dev";
    ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);

    command = "-";
    ret = exchange_directory(command);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("/home", getenv("PWD"));
}


/**
 * @brief run at the end of tests.
 */
void setUp(void)
{
}

/**
 * @brief run at the begining of tests.
 */
void tearDown(void)
{
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_test);
    RUN_TEST(test_generate_prompt);

    RUN_TEST(test_msg_null);
    RUN_TEST(test_bufflen_zero);
    RUN_TEST(test_env_var_exists);
    RUN_TEST(test_env_var_not_exists);
    RUN_TEST(test_env_var_not_exists_small_buffer);
    RUN_TEST(test_valid_comment);
    RUN_TEST(test_normal_message);

    RUN_TEST(test_exchange_directory);
    RUN_TEST(test_exchange_directory_fail);
    RUN_TEST(test_exchange_directory_pass);
    RUN_TEST(test_exchange_directory_pass2);
    RUN_TEST(test_exchange_directory_pass3);



    return UNITY_END();
}
