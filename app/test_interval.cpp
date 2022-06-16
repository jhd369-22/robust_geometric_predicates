#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <ra/interval.hpp>

TEMPLATE_TEST_CASE("default constructor", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i;
    CHECK(i.lower() == TestType(0));
    CHECK(i.upper() == TestType(0));

    ra::math::interval<TestType> i2(TestType(3));
    CHECK(i2.lower() == TestType(3));
    CHECK(i2.upper() == TestType(3));
}

TEMPLATE_TEST_CASE("copy constructor", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3));
    ra::math::interval<TestType> i2(i);
    CHECK(i2.lower() == TestType(3));
    CHECK(i2.upper() == TestType(3));
}

TEMPLATE_TEST_CASE("copy assignment operator", "[interval]", float, double, long double) {
    SECTION("assigned from other interval") {
        ra::math::interval<TestType> i(TestType(3));
        ra::math::interval<TestType> i2;
        i2 = i;
        CHECK(i2.lower() == TestType(3));
        CHECK(i2.upper() == TestType(3));
    }

    SECTION("assigned from the same interval") {
        ra::math::interval<TestType> i(TestType(3));
        i = i;
        CHECK(i.lower() == TestType(3));
        CHECK(i.upper() == TestType(3));
    }
}

TEMPLATE_TEST_CASE("move constructor", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3));
    ra::math::interval<TestType> i2(std::move(i));
    CHECK(i2.lower() == TestType(3));
    CHECK(i2.upper() == TestType(3));
}

TEMPLATE_TEST_CASE("move assignment operator", "[interval]", float, double, long double) {
    SECTION("assigned from other interval") {
        ra::math::interval<TestType> i(TestType(3));
        ra::math::interval<TestType> i2;
        i2 = std::move(i);
        CHECK(i2.lower() == TestType(3));
        CHECK(i2.upper() == TestType(3));
    }

    SECTION("assigned from the same interval") {
        ra::math::interval<TestType> i(TestType(3));
        i = std::move(i);
        CHECK(i.lower() == TestType(3));
        CHECK(i.upper() == TestType(3));
    }
}

TEMPLATE_TEST_CASE("two-argument constructor", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3), TestType(6));
    CHECK(i.lower() == TestType(3));
    CHECK(i.upper() == TestType(6));
}

TEMPLATE_TEST_CASE("operator+=", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3), TestType(6));
    ra::math::interval<TestType> i2(TestType(3), TestType(6));
    i += i2;
    CHECK(i.lower() == TestType(6));
    CHECK(i.upper() == TestType(12));

    ra::math::interval<TestType>::statistics stats;
    ra::math::interval<TestType>::get_statistics(stats);
    CHECK(stats.arithmetic_op_count == 1);
    CHECK(stats.interval_op_count == 0);
}

TEMPLATE_TEST_CASE("operator-=", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3), TestType(6));
    ra::math::interval<TestType> i2(TestType(3), TestType(6));
    i -= i2;
    CHECK(i.lower() == TestType(-3));
    CHECK(i.upper() == TestType(3));

    ra::math::interval<TestType>::statistics stats;
    ra::math::interval<TestType>::get_statistics(stats);
    CHECK(stats.arithmetic_op_count == 2);
    CHECK(stats.interval_op_count == 0);
}

TEMPLATE_TEST_CASE("operator*=", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3), TestType(6));
    ra::math::interval<TestType> i2(TestType(3), TestType(6));
    i *= i2;
    CHECK(i.lower() == TestType(9));
    CHECK(i.upper() == TestType(36));

    ra::math::interval<TestType>::statistics stats;
    ra::math::interval<TestType>::get_statistics(stats);
    CHECK(stats.arithmetic_op_count == 3);
    CHECK(stats.interval_op_count == 0);
}

TEMPLATE_TEST_CASE("is_singleton", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(3), TestType(6));
    CHECK(i.is_singleton() == false);

    ra::math::interval<TestType> i2(TestType(3));
    CHECK(i2.is_singleton() == true);
}

TEMPLATE_TEST_CASE("sign", "[interval]", float, double, long double) {
    ra::math::interval<TestType> i(TestType(-6), TestType(-3));
    CHECK(i.sign() == -1);

    ra::math::interval<TestType> i2(TestType(3), TestType(6));
    CHECK(i2.sign() == 1);

    ra::math::interval<TestType> i3;
    CHECK(i3.sign() == 0);
}

TEMPLATE_TEST_CASE("clear_statistics && get_statistics", "[interval]", float, double, long double) {
    //ra::math::interval<TestType> i(TestType(3), TestType(6));
    //i.clear_statistics();
}