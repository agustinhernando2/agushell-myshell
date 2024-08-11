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
    snprintf(buffer, sizeof(buffer), AMARILLO "%s@" GRIS "%s:~%s$" CELESTE ">", username, hostname, pwd);

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
    TEST_ASSERT_EQUAL_STRING(ROJO "Environment not found: NO_EXISTE", buffer);
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


    return UNITY_END();
}
