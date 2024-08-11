#include <unity.h>
#include <functions.h>

void test_test(){
    TEST_ASSERT_EQUAL(0,0);
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


    return UNITY_END();
}
