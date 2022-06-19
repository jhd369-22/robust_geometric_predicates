#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>

#include <cstddef>
#include <ra/interval.hpp>

namespace ra::geometry {

    // A geometry kernel with robust predicates.
    template <class R>
    class Kernel {
        public:
            // The type used to represent real numbers.
            using Real = R;

            // The type used to represent points in two dimensions.
            using Point = typename CGAL::Cartesian<R>::Point_2;

            // The type used to represent vectors in two dimensions.
            using Vector = typename CGAL::Cartesian<R>::Vector_2;

            // The possible outcomes of an orientation test.
            enum class Orientation : int {
                right_turn = -1,
                collinear = 0,
                left_turn = 1,
            };

            // The possible outcomes of an oriented-side-of test.
            enum class Oriented_side : int {
                on_negative_side = -1,
                on_boundary = 0,
                on_positive_side = 1,
            };

            // The set of statistics maintained by the kernel.
            struct Statistics {
                    // The total number of orientation tests.
                    std::size_t orientation_total_count;
                    // The number of orientation tests requiring exact
                    // arithmetic.
                    std::size_t orientation_exact_count;
                    // The total number of preferred-direction tests.
                    std::size_t preferred_direction_total_count;
                    // The number of preferred-direction tests requiring
                    // exact arithmetic.
                    std::size_t preferred_direction_exact_count;
                    // The total number of side-of-oriented-circle tests.
                    std::size_t side_of_oriented_circle_total_count;
                    // The number of side-of-oriented-circle tests
                    // requiring exact arithmetic.
                    std::size_t side_of_oriented_circle_exact_count;
            };

            // Since a kernel object is stateless, construction and
            // destruction are trivial.
            Kernel() = default;
            ~Kernel() = default;

            // The kernel type is both movable and copyable.
            // Since a kernel object is stateless, a copy/move operation
            // is trivial.
            Kernel(const Kernel&) = default;
            Kernel& operator=(const Kernel&) = default;
            Kernel(Kernel&&) = default;
            Kernel& operator=(Kernel&&) = default;

            // Determines how the point c is positioned relative to the
            // directed line through the points a and b (in that order).
            // Precondition: The points a and b have distinct values.
            Orientation orientation(const Point& a, const Point& b, const Point& c) {
                ra::math::interval<Real> ax(a.x()), ay(a.y());
                ra::math::interval<Real> bx(b.x()), by(b.y());
                ra::math::interval<Real> cx(c.x()), cy(c.y());

                // The orientation test is determined by the
                // sign of the determinant of the following matrix:
                ra::math::interval<Real> orient2d = (ax - cx) * (by - cy) - (bx - cx) * (ay - cy);

                ++stats_.orientation_total_count;

                try {
                    switch (orient2d.sign()) {
                        case -1:
                            return Orientation::right_turn;
                        case 0:
                            return Orientation::collinear;
                        case 1:
                            return Orientation::left_turn;
                    }
                } catch (std::runtime_error& e) {
                    ++stats_.orientation_exact_count;

                    // Use the exact arithmetic
                    CGAL::MP_Float exact_ax(a.x()), exact_ay(a.y());
                    CGAL::MP_Float exact_bx(b.x()), exact_by(b.y());
                    CGAL::MP_Float exact_cx(c.x()), exact_cy(c.y());

                    CGAL::MP_Float exact_orient2d = (exact_ax - exact_cx) * (exact_by - exact_cy) - (exact_bx - exact_cx) * (exact_ay - exact_cy);

                    if (exact_orient2d < 0) {
                        return Orientation::right_turn;
                    } else if (exact_orient2d == 0) {
                        return Orientation::collinear;
                    } else {
                        return Orientation::left_turn;
                    }
                }
            }

            // Determines how the point d is positioned relative to the
            // oriented circle passing through the points a, b, and c
            // (in that order).
            // Precondition: The points a, b, and c are not collinear.
            Oriented_side side_of_oriented_circle(const Point& a, const Point& b,
                                                  const Point& c, const Point& d) {
                ra::math::interval<Real> ax(a.x()), ay(a.y());
                ra::math::interval<Real> az(ax * ax + ay * ay);

                ra::math::interval<Real> bx(b.x()), by(b.y());
                ra::math::interval<Real> bz(bx * bx + by * by);

                ra::math::interval<Real> cx(c.x()), cy(c.y());
                ra::math::interval<Real> cz(cx * cx + cy * cy);

                ra::math::interval<Real> dx(d.x()), dy(d.y());
                ra::math::interval<Real> dz(dx * dx + dy * dy);

                // The side of the oriented circle is determined by the
                // sign of the determinant of the following matrix:
                ra::math::interval<Real> inCircle = ax * (by * cz - bz * cy) -
                                                    bx * (ay * cz - az * cy) +
                                                    cx * (ay * bz - az * by);

                ++stats_.side_of_oriented_circle_total_count;

                try {
                    switch (inCircle.sign()) {
                        case -1:
                            return Oriented_side::on_negative_side;
                        case 0:
                            return Oriented_side::on_boundary;
                        case 1:
                            return Oriented_side::on_positive_side;
                    }
                } catch (std::runtime_error& e) {
                    ++stats_.side_of_oriented_circle_exact_count;

                    // Use the exact arithmetic
                    CGAL::MP_Float exact_ax(a.x()), exact_ay(a.y());
                    CGAL::MP_Float exact_az(exact_ax * exact_ax + exact_ay * exact_ay);

                    CGAL::MP_Float exact_bx(b.x()), exact_by(b.y());
                    CGAL::MP_Float exact_bz(exact_bx * exact_bx + exact_by * exact_by);

                    CGAL::MP_Float exact_cx(c.x()), exact_cy(c.y());
                    CGAL::MP_Float exact_cz(exact_cx * exact_cx + exact_cy * exact_cy);

                    CGAL::MP_Float exact_dx(d.x()), exact_dy(d.y());
                    CGAL::MP_Float exact_dz(exact_dx * exact_dx + exact_dy * exact_dy);

                    CGAL::MP_Float exact_inCircle = exact_ax * (exact_by * exact_cz - exact_bz * exact_cy) -
                                                    exact_bx * (exact_ay * exact_cz - exact_az * exact_cy) +
                                                    exact_cx * (exact_ay * exact_bz - exact_az * exact_by);

                    if (exact_inCircle < 0) {
                        return Oriented_side::on_negative_side;
                    } else if (exact_inCircle == 0) {
                        return Oriented_side::on_boundary;
                    } else {
                        return Oriented_side::on_positive_side;
                    }
                }
            }

            // Determines if, compared to the orientation of line
            // segment cd, the orientation of the line segment ab is
            // more close, equally close, or less close to the
            // orientation of the vector v.
            // The value returned is 1, 0, or -1 if, compared to the
            // orientation of cd, the orientation of ab is more close,
            // equally close, or less close to the orientation of v,
            // respectively.
            // Precondition: The points a and b have distinct values; the
            // points c and d have distinct values; the vector v is not
            // the zero vector.
            int preferred_direction(const Point& a, const Point& b,
                                    const Point& c, const Point& d, const Vector& v) {
                ra::math::interval<Real> ax(a.x()), ay(a.y());

                ra::math::interval<Real> bx(b.x()), by(b.y());

                ra::math::interval<Real> cx(c.x()), cy(c.y());

                ra::math::interval<Real> dx(d.x()), dy(d.y());

                ra::math::interval<Real> vx(v.x()), vy(v.y());

                ra::math::interval<Real> prefDir = ((dx - cx) * (dx - cx) + (dy - cy) * (dy - cy)) *
                                                       (((bx - ax) * vx + (by - ay) * vy) * ((bx - ax) * vx + (by - ay) * vy)) -
                                                   ((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) *
                                                       (((dx - cx) * vx + (dy - cy) * vy) * ((dx - cx) * vx + (dy - cy) * vy));

                ++stats_.preferred_direction_total_count;

                try {
                    switch (prefDir.sign()) {
                        case -1:
                            return -1;
                        case 0:
                            return 0;
                        case 1:
                            return 1;
                    }
                } catch (std::runtime_error& e) {
                    ++stats_.preferred_direction_exact_count;

                    // Use the exact arithmetic
                    CGAL::MP_Float exact_ax(a.x()), exact_ay(a.y());

                    CGAL::MP_Float exact_bx(b.x()), exact_by(b.y());

                    CGAL::MP_Float exact_cx(c.x()), exact_cy(c.y());

                    CGAL::MP_Float exact_dx(d.x()), exact_dy(d.y());

                    CGAL::MP_Float exact_vx(v.x()), exact_vy(v.y());

                    CGAL::MP_Float exact_prefDir = ((exact_dx - exact_cx) * (exact_dx - exact_cx) + (exact_dy - exact_cy) * (exact_dy - exact_cy)) *
                                                       (((exact_bx - exact_ax) * exact_vx + (exact_by - exact_ay) * exact_vy) * ((exact_bx - exact_ax) * exact_vx + (exact_by - exact_ay) * exact_vy)) -
                                                   ((exact_bx - exact_ax) * (exact_bx - exact_ax) + (exact_by - exact_ay) * (exact_by - exact_ay)) *
                                                       (((exact_dx - exact_cx) * exact_vx + (exact_dy - exact_cy) * exact_vy) * ((exact_dx - exact_cx) * exact_vx + (exact_dy - exact_cy) * exact_vy));

                    if (exact_prefDir < 0) {
                        return -1;
                    } else if (exact_prefDir == 0) {
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }

            // Tests if the quadrilateral with vertices a, b, c, and d
            // specified in CCW order is strictly convex.
            // Precondition: The vertices a, b, c, and d have distinct
            // values and are specified in CCW order.
            bool is_strictly_convex_quad(const Point& a, const Point& b,
                                         const Point& c, const Point& d) {
                Orientation o1 = orientation(a, b, c);
                Orientation o2 = orientation(b, c, d);
                Orientation o3 = orientation(c, d, a);
                Orientation o4 = orientation(d, a, b);

                if (o1 == Orientation::left_turn && o2 == Orientation::left_turn &&
                    o3 == Orientation::left_turn && o4 == Orientation::left_turn) {
                    return true;
                } else {
                    return false;
                }
            }

            // Tests if the flippable edge, with endpoints a and c and
            // two incident faces abc and acd, is locally Delaunay.
            // Precondition: The points a, b, c, and d have distinct
            // values; the quadrilateral abcd must be strictly convex.
            bool is_locally_delaunay_edge(const Point& a, const Point& b,
                                          const Point& c, const Point& d) {
                Oriented_side o1 = side_of_oriented_circle(a, b, c, d);

                if (o1 == Oriented_side::on_negative_side || o1 == Oriented_side::on_boundary) {
                    return true;
                } else {
                    return false;
                }
            }

            // Tests if the flippable edge, with endpoints a and c and
            // two incident faces abc and acd, has the preferred-directions
            // locally-Delaunay property with respect to the first and
            // second directions u and v.
            // Precondition: The points a, b, c, and d have distinct values;
            // the vectors u and v are not zero vectors; the vectors u and
            // v are neither parallel nor orthogonal.
            bool is_locally_pd_delaunay_edge(const Point& a, const Point& b, const Point& c,
                                             const Point& d, const Vector& u, const Vector& v) {
                int pd1 = preferred_direction(a, b, c, d, u);
                int pd2 = preferred_direction(a, b, c, d, v);
                bool alpha = (pd1 == 1 || (pd1 == 0 && pd2 == 1));

                Oriented_side o1 = side_of_oriented_circle(a, b, c, d);

                switch (o1) {
                    case Oriented_side::on_negative_side:
                        return true;
                    case Oriented_side::on_positive_side:
                        return false;
                    case Oriented_side::on_boundary:
                        return alpha;
                }
            }

            // Clear (i.e., set to zero) all kernel statistics.
            static void clear_statistics() {
                stats_ = Statistics{};
            }

            // Get the current values of the kernel statistics.
            static void get_statistics(Statistics& statistics) {
                statistics = stats_;
            }

        private:
            inline static Statistics stats_{};
    };
}  // namespace ra::geometry