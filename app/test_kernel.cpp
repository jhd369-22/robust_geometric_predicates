#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <ra/kernel.hpp>
#include <sstream>

TEMPLATE_TEST_CASE("Kernel basic", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    typename Kernel::Point p1(TestType(1.25), TestType(2.36));
    CHECK(p1.x() == TestType(1.25));
    CHECK(p1.y() == TestType(2.36));

    typename Kernel::Vector v1(TestType(3.33), TestType(4.56));
    CHECK(v1.x() == TestType(3.33));
    CHECK(v1.y() == TestType(4.56));

    typename Kernel::Statistics stats;
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

    Kernel k;
    Kernel::clear_statistics();

    SECTION("orientation with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(2));
        typename Kernel::Point c(TestType(2), TestType(0));
        typename Kernel::Point d(TestType(1), TestType(1));
        typename Kernel::Point e(TestType(0), TestType(2));

        CHECK(k.orientation(a, b, c) == Kernel::Orientation::right_turn);
        CHECK(k.orientation(a, b, d) == Kernel::Orientation::collinear);
        CHECK(k.orientation(a, b, e) == Kernel::Orientation::left_turn);

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.orientation_total_count == std::size_t(3));
        CHECK(stats.orientation_exact_count == std::size_t(0));
    }

//     SECTION("orientation with exact") {
//         typename Kernel::Point a(TestType(0), TestType(0));
//         typename Kernel::Point b(TestType(2), TestType(2));
//         typename Kernel::Point c(TestType(1.000001), TestType(1));
//         typename Kernel::Point d(TestType(0.999999), TestType(1));

//         CHECK(k.orientation(a, b, c) == Kernel::Orientation::right_turn);
//         CHECK(k.orientation(a, b, d) == Kernel::Orientation::left_turn);

//         typename Kernel::Statistics stats;
//         Kernel::get_statistics(stats);

//         CHECK(stats.orientation_total_count == std::size_t(2));
//         CHECK(stats.orientation_exact_count == std::size_t(2));
//     }
}

TEMPLATE_TEST_CASE("side_of_oriented_circle", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel k;
    Kernel::clear_statistics();

    SECTION("side_of_oriented_circle with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(0));
        typename Kernel::Point c(TestType(0), TestType(2));
        typename Kernel::Point d(TestType(1), TestType(1));
        typename Kernel::Point e(TestType(2), TestType(2));
        typename Kernel::Point f(TestType(3), TestType(3));

        CHECK(k.side_of_oriented_circle(a, b, c, d) == Kernel::Oriented_side::on_positive_side);
        CHECK(k.side_of_oriented_circle(a, b, c, e) == Kernel::Oriented_side::on_boundary);
        CHECK(k.side_of_oriented_circle(a, b, c, f) == Kernel::Oriented_side::on_negative_side);

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(3));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(0));
    }

    // SECTION("side_of_oriented_circle with exact") {
    //     typename Kernel::Point a(TestType(0), TestType(0));
    //     typename Kernel::Point b(TestType(2), TestType(0));
    //     typename Kernel::Point c(TestType(0), TestType(2));
    //     typename Kernel::Point d(TestType(2.0000003), TestType(2));
    //     typename Kernel::Point e(TestType(2.0000002), TestType(2));

    //     CHECK(k.side_of_oriented_circle(a, b, c, d) == Kernel::Oriented_side::on_negative_side);
    //     CHECK(k.side_of_oriented_circle(a, b, c, e) == Kernel::Oriented_side::on_negative_side);

    //     typename Kernel::Statistics stats;
    //     Kernel::get_statistics(stats);

    //     CHECK(stats.side_of_oriented_circle_total_count == std::size_t(2));
    //     CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(2));
    // }
}

TEMPLATE_TEST_CASE("preferred_direction", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel k;
    Kernel::clear_statistics();

    SECTION("preferred_direction with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(2));
        typename Kernel::Point c(TestType(2), TestType(0));
        typename Kernel::Point d(TestType(0), TestType(2));
        typename Kernel::Vector u(TestType(2), TestType(1));
        typename Kernel::Vector v(TestType(1), TestType(0));
        typename Kernel::Vector w(TestType(-1), TestType(2));

        CHECK(k.preferred_direction(a, b, c, d, u) == int(1));
        CHECK(k.preferred_direction(a, b, c, d, v) == int(0));
        CHECK(k.preferred_direction(a, b, c, d, w) == int(-1));

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.preferred_direction_total_count == std::size_t(3));
        CHECK(stats.preferred_direction_exact_count == std::size_t(0));
    }

    // SECTION("preferred_direction with exact") {
    //     typename Kernel::Point a(TestType(0), TestType(0));
    //     typename Kernel::Point b(TestType(2), TestType(2));
    //     typename Kernel::Point c(TestType(2), TestType(0));
    //     typename Kernel::Point d(TestType(0), TestType(2));
    //     typename Kernel::Vector u(TestType(1), TestType(0.000001));
    //     typename Kernel::Vector v(TestType(1), TestType(-0.000001));

    //     CHECK(k.preferred_direction(a, b, c, d, u) == int(1));
    //     CHECK(k.preferred_direction(a, b, c, d, v) == int(-1));

    //     typename Kernel::Statistics stats;
    //     Kernel::get_statistics(stats);

    //     CHECK(stats.preferred_direction_total_count == std::size_t(2));
    //     CHECK(stats.preferred_direction_exact_count == std::size_t(2));
    // }
}

TEMPLATE_TEST_CASE("is_strictly_convex_quadrilateral", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel k;
    Kernel::clear_statistics();

    SECTION("is_strictly_convex_quadrilateral with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(0));
        typename Kernel::Point c(TestType(2), TestType(2));
        typename Kernel::Point d(TestType(0), TestType(2));
        typename Kernel::Point e(TestType(1.5), TestType(1));
        typename Kernel::Point f(TestType(0.3), TestType(0.3));

        CHECK(k.is_strictly_convex_quad(a, b, c, d) == true);
        CHECK(k.is_strictly_convex_quad(a, b, c, e) == false);
        CHECK(k.is_strictly_convex_quad(a, b, c, f) == false);

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.orientation_total_count == std::size_t(12));
        CHECK(stats.orientation_exact_count == std::size_t(0));
    }

    // SECTION("is_strictly_convex_quadrilateral with exact") {
    //     typename Kernel::Point a(TestType(0), TestType(0));
    //     typename Kernel::Point b(TestType(2), TestType(0));
    //     typename Kernel::Point c(TestType(2), TestType(2));
    //     typename Kernel::Point d(TestType(1.50001), TestType(1.5));
    //     typename Kernel::Point e(TestType(1.49999), TestType(1.5));

    //     CHECK(k.is_strictly_convex_quad(a, b, c, d) == true);
    //     CHECK(k.is_strictly_convex_quad(a, b, c, e) == false);

    //     typename Kernel::Statistics stats;
    //     Kernel::get_statistics(stats);

    //     CHECK(stats.orientation_total_count == std::size_t(8));
    //     CHECK(stats.orientation_exact_count == std::size_t(2));
    // }
}

TEMPLATE_TEST_CASE("is_locally_delaunay_edge", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel k;
    Kernel::clear_statistics();

    SECTION("is_locally_delaunay_edge with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(0));
        typename Kernel::Point c(TestType(0), TestType(2));
        typename Kernel::Point d(TestType(1), TestType(1));
        typename Kernel::Point e(TestType(2), TestType(2));
        typename Kernel::Point f(TestType(3), TestType(3));

        CHECK(k.is_locally_delaunay_edge(a, b, c, d) == false);
        CHECK(k.is_locally_delaunay_edge(a, b, c, e) == true);
        CHECK(k.is_locally_delaunay_edge(a, b, c, f) == true);

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(3));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(0));
    }

    // SECTION("is_locally_delaunay_edge with exact") {
    //     typename Kernel::Point a(TestType(0), TestType(0));
    //     typename Kernel::Point b(TestType(2), TestType(0));
    //     typename Kernel::Point c(TestType(0), TestType(2));
    //     typename Kernel::Point d(TestType(1.9999999999999999999), TestType(2));
    //     typename Kernel::Point e(TestType(2.0000000000000000001), TestType(2));

    //     CHECK(k.is_locally_delaunay_edge(a, b, c, d) == false);
    //     CHECK(k.is_locally_delaunay_edge(a, b, c, e) == true);

    //     typename Kernel::Statistics stats;
    //     Kernel::get_statistics(stats);

    //     CHECK(stats.side_of_oriented_circle_total_count == std::size_t(2));
    //     CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(2));
    // }
}

TEMPLATE_TEST_CASE("is_locally_pd_delaunay_edge", "[Kernel]", float, double) {
    namespace rg = ra::geometry;
    using Kernel = typename rg::Kernel<TestType>;

    Kernel k;
    Kernel::clear_statistics();

    SECTION("is_locally_pd_delaunay_edge with interval") {
        typename Kernel::Point a(TestType(0), TestType(0));
        typename Kernel::Point b(TestType(2), TestType(2));
        typename Kernel::Point c(TestType(2), TestType(0));
        typename Kernel::Point d(TestType(0), TestType(2));
        typename Kernel::Vector u(TestType(2), TestType(1));
        typename Kernel::Vector v(TestType(1), TestType(0));
        typename Kernel::Vector w(TestType(-1), TestType(2));
        typename Kernel::Vector x(TestType(1), TestType(1));
        typename Kernel::Vector y(TestType(3), TestType(3));

        CHECK(k.is_locally_pd_delaunay_edge(a, b, c, d, u, v) == true);
        CHECK(k.is_locally_pd_delaunay_edge(a, b, c, d, v, u) == true);
        CHECK(k.is_locally_pd_delaunay_edge(a, b, c, d, w, v) == false);

        CHECK(k.is_locally_pd_delaunay_edge(a, c, d, x, u, v) == false);
        CHECK(k.is_locally_pd_delaunay_edge(a, c, d, x, v, u) == false);
        CHECK(k.is_locally_pd_delaunay_edge(a, c, d, y, w, v) == true);

        typename Kernel::Statistics stats;
        Kernel::get_statistics(stats);

        CHECK(stats.preferred_direction_total_count == std::size_t(6));
        CHECK(stats.preferred_direction_exact_count == std::size_t(0));
        CHECK(stats.side_of_oriented_circle_total_count == std::size_t(3));
        CHECK(stats.side_of_oriented_circle_exact_count == std::size_t(0));
    }
}