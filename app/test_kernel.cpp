#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <ra/kernel.hpp>
#include <sstream>

TEMPLATE_TEST_CASE("Kernel basic", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Point p1(TestType(1.25), TestType(2.36));
    CHECK(p1.x() == TestType(1.25));
    CHECK(p1.y() == TestType(2.36));

    Kernel::Vector v1(TestType(3.33), TestType(4.56));
    CHECK(v1.x() == TestType(3.33));
    CHECK(v1.y() == TestType(4.56));

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    CHECK(stats.orientation_total_count == std::size_t(0));
    CHECK(stats.orientation_exact_count == std::size_t(0));
    CHECK(stats.preferred_direction_total_count == std::size_t(0));
    CHECK(stats.preferred_direction_exact_count == std::size_t(0));
    CHECK(stats.side_of_oriented_circle_total_count == std::size_t(0));
    CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(0));
}

TEMPLATE_TEST_CASE("orientation", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    SECTION("orientation with interval") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(2));
        Kernel::Point c(TestType(2), TestType(0));
        Kernel::Point d(TestType(1), TestType(1));
        Kernel::Point e(TestType(0), TestType(2));

        CHECK(Kernel::orientation(a, b, c) == Kernel::Orientation::right_turn);
        CHECK(Kernel::orientation(a, b, d) == Kernel::Orientation::collinear);
        CHECK(Kernel::orientation(a, b, e) == Kernel::Orientation::left_turn);

        CHECK(stats.orientation_total_count == std::size_t(3));
        CHECK(stats.orientation_exact_count == std::size_t(0));
    }

    SECTION("orientation with exact") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(2));
        Kernel::Point c(TestType(1.0000000000000000001), TestType(1));
        Kernel::Point d(TestType(0.9999999999999999999), TestType(1));

        CHECK(Kernel::orientation(a, b, c) == Kernel::Orientation::right_turn);
        CHECK(Kernel::orientation(a, b, d) == Kernel::Orientation::left_turn);

        CHECK(stats.orientation_total_count == std::size_t(5));
        CHECK(stats.orientation_exact_count == std::size_t(2));
    }
}

TEMPLATE_TEST_CASE("side_of_oriented_circle", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    SECTION("side_of_oriented_circle with interval") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(0), TestType(2));
        Kernel::Point d(TestType(1), TestType(1));
        Kernel::Point e(TestType(2), TestType(2));
        Kernel::Point f(TestType(3), TestType(3));

        CHECK(Kernel::side_of_oriented_circle(a, b, c, d) == Kernel::Oriented_side::on_positive_side);
        CHECK(Kernel::side_of_oriented_circle(a, b, c, e) == Kernel::Oriented_side::on_boundary);
        CHECK(Kernel::side_of_oriented_circle(a, b, c, f) == Kernel::Oriented_side::on_negative_side);

        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(3));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(0));
    }

    SECTION("side_of_oriented_circle with exact") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(0), TestType(2));
        Kernel::Point d(TestType(1.9999999999999999999), TestType(2));
        Kernel::Point e(TestType(2.0000000000000000001), TestType(2));

        CHECK(Kernel::side_of_oriented_circle(a, b, c, d) == Kernel::Oriented_side::on_positive_side);
        CHECK(Kernel::side_of_oriented_circle(a, b, c, e) == Kernel::Oriented_side::on_negative_side);

        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(5));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(2));
    }
}

TEMPLATE_TEST_CASE("preferred_direction", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    SECTION("preferred_direction with interval") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(2));
        Kernel::Point c(TestType(2), TestType(0));
        Kernel::Point d(TestType(0), TestType(2));
        Kernel::Vector u(TestType(2), TestType(1));
        Kernel::Vector v(TestType(1), TestType(0));
        Kernel::Vector w(TestType(-1), TestType(2));

        CHECK(Kernel::preferred_direction(a, b, c, u) == int(1));
        CHECK(Kernel::preferred_direction(a, b, c, v) == int(0));
        CHECK(Kernel::preferred_direction(a, b, c, w) == int(-1));

        CHECK(stats.preferred_direction_total_count == std::size_t(3));
        CHECK(stats.preferred_direction_exact_count == std::size_t(0));
    }

    SECTION("preferred_direction with exact") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(2));
        Kernel::Point c(TestType(2), TestType(0));
        Kernel::Point d(TestType(0), TestType(2));
        Kernel::Vector u(TestType(1), TestType(0.0000000000000000001));
        Kernel::Vector v(TestType(1), TestType(-0.0000000000000000001));

        CHECK(Kernel::preferred_direction(a, b, c, u) == int(1));
        CHECK(Kernel::preferred_direction(a, b, c, v) == int(-1));

        CHECK(stats.preferred_direction_total_count == std::size_t(5));
        CHECK(stats.preferred_direction_exact_count == std::size_t(2));
    }
}

TEMPLATE_TEST_CASE("is_strictly_convex_quadrilateral", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    SECTION("is_strictly_convex_quadrilateral with interval") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(2), TestType(2));
        Kernel::Point d(TestType(0), TestType(2));
        Kernel::Point e(TestType(1.5), TestType(1));
        Kernel::Point f(TestType(0.3), TestType(0.3));

        CHECK(Kernel::is_strictly_convex_quad(a, b, c, d) == true);
        CHECK(Kernel::is_strictly_convex_quad(a, b, c, e) == false);
        CHECK(Kernel::is_strictly_convex_quad(a, b, c, f) == false);

        CHECK(stats.orientation_total_count == std::size_t(3));
        CHECK(stats.orientation_exact_count == std::size_t(0));
    }

    SECTION("is_strictly_convex_quadrilateral with exact") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(2), TestType(2));
        Kernel::Point d(TestType(1.5000000000000000001), TestType(1.5));
        Kernel::Point e(TestType(1.4999999999999999999), TestType(1.5));

        CHECK(Kernel::is_strictly_convex_quad(a, b, c, d) == true);
        CHECK(Kernel::is_strictly_convex_quad(a, b, c, e) == false);

        CHECK(stats.orientation_total_count == std::size_t(5));
        CHECK(stats.orientation_exact_count == std::size_t(2));
    }
}

TEMPLATE_TEST_CASE("is_locally_delaunay_edge", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel::Statistics stats;
    Kernel::clear_statistics();
    Kernel::get_statistics(stats);

    SECTION("is_locally_delaunay_edge with interval") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(0), TestType(2));
        Kernel::Point d(TestType(1), TestType(1));
        Kernel::Point e(TestType(2), TestType(2));
        Kernel::Point f(TestType(3), TestType(3));

        CHECK(Kernel::is_locally_delaunay_edge(a, b, c, d) == false);
        CHECK(Kernel::is_locally_delaunay_edge(a, b, c, e) == true);
        CHECK(Kernel::is_locally_delaunay_edge(a, b, c, f) == true);

        CHECK(stats.orientation_total_count == std::size_t(3));
        CHECK(stats.orientation_exact_count == std::size_t(0));
    }

    SECTION("is_locally_delaunay_edge with exact") {
        Kernel::Point a(TestType(0), TestType(0));
        Kernel::Point b(TestType(2), TestType(0));
        Kernel::Point c(TestType(0), TestType(2));
        Kernel::Point d(TestType(1.9999999999999999999), TestType(2));
        Kernel::Point e(TestType(2.0000000000000000001), TestType(2));

        CHECK(Kernel::is_locally_delaunay_edge(a, b, c, d) == false);
        CHECK(Kernel::is_locally_delaunay_edge(a, b, c, e) == true);

        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(5));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(2));
    }
}

TEMPLATE_TEST_CASE("is_locally_pd_delaunay_edge", "[Kernel]", float, double) {
    
}