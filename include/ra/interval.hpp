#include <algorithm>
#include <stdexcept>

#pragma STDC FENV_ACCESS ON

namespace ra::math {
    struct indeterminate_result : public std::runtime_error {
            using std::runtime_error::runtime_error;
    };

    template <typename T>
    class interval {
        public:
            using real_type = T;

            struct statistics {
                    // The total number of indeterminate results encountered.
                    unsigned long indeterminate_result_count;
                    // The total number of interval arithmetic operations.
                    unsigned long arithmetic_op_count;
            };

            // default constructor
            interval(real_type r = real_type(0)) : lower_(r), upper_(r), stats_() {}

            // copy constructor
            interval(const interval& other) : lower_(other.lower_), upper_(other.upper_), stats_(other.stats_) {}

            // copy assignment operator
            interval& operator=(const interval& other) {
                if (this != &other) {
                    lower_ = other.lower_;
                    upper_ = other.upper_;
                    stats_ = other.stats_;
                }

                return *this;
            }

            // move constructor
            interval(interval&& other) : lower_(other.lower_), upper_(other.upper_), stats_(other.stats_) {}

            // move assignment operator
            interval& operator=(interval&& other) {
                if (this != &other) {
                    lower_ = other.lower_;
                    upper_ = other.upper_;
                    stats_ = other.stats_;
                }

                return *this;
            }

            // two-argument constructor
            interval(real_type lower, real_type upper) {
                if (lower > upper) {
                    // TODO: If this precondition is violated, the behavior of this constructor is implementation defined.
                }

                lower_ = lower;
                upper_ = upper;

                stats_.indeterminate_result_count = 0;
                stats_.arithmetic_op_count = 0;
            }

            // destructor
            ~interval() = default;

            // floating-point interval compound assignment operators
            interval& operator+=(const interval& other) {
                lower_ += other.lower_;
                upper_ += other.upper_;
                ++stats_.arithmetic_op_count;

                return *this;
            }

            interval& operator-=(const interval& other) {
                lower_ -= other.upper_;
                upper_ -= other.lower_;
                ++stats_.arithmetic_op_count;

                return *this;
            }

            interval& operator*=(const interval& other) {
                real_type lower = std::min(std::min(lower_ * other.lower_, lower_ * other.upper_),
                                           std::min(upper_ * other.lower_, upper_ * other.upper_));

                real_type upper = std::max(std::max(lower_ * other.lower_, lower_ * other.upper_),
                                           std::max(upper_ * other.lower_, upper_ * other.upper_));

                lower_ = lower;
                upper_ = upper;
                ++stats_.arithmetic_op_count;

                return *this;
            }

            // return lower bound
            real_type lower() const { return lower_; }

            // return upper bound
            real_type upper() const { return upper_; }

            bool is_singleton() const { return lower_ == upper_; }

            int sign() const {
                if (lower_ < 0 && upper_ < 0) {
                    return -1;
                } else if (lower_ > 0 && upper_ > 0) {
                    return 1;
                } else if (lower_ == 0 && upper_ == 0) {
                    return 0;
                } else {
                    ++stats_.indeterminate_result_count;
                    throw indeterminate_result("Indeterminate result");
                }
            }

            // clear statistics
            static void clear_statistics() {
                stats_.indeterminate_result_count = 0;
                stats_.arithmetic_op_count = 0;
            }

            // get statistics
            static void get_statistics(statistics& stat) { stat = stats_; }

        private:
            real_type lower_;
            real_type upper_;
            statistics stats_;
    };

    // binary operations
    // TODO: Which interval objects should increase the arithmetic operation count?
    template <typename T>
    interval<T> operator+(const interval<T>& lhs, const interval<T>& rhs) {
        interval<T> result(lhs);
        result += rhs;
        
        return result;
    }

    template <typename T>
    interval<T> operator-(const interval<T>& lhs, const interval<T>& rhs) {
        interval<T> result(lhs);
        result -= rhs;

        return result;
    }

    template <typename T>
    interval<T> operator*(const interval<T>& lhs, const interval<T>& rhs) {
        interval<T> result(lhs);
        result *= rhs;

        return result;
    }

    // less-than comparison
    template <typename T>
    bool operator<(const interval<T>& lhs, const interval<T>& rhs) {
        if(lhs.lower() < rhs.lower() && lhs.upper() < rhs.upper()) {
            return true;
        } else if(lhs.lower() >= rhs.lower() && lhs.upper() >= rhs.upper()) {
            return false;
        } else {
            ++stats_.indeterminate_result_count;
            throw indeterminate_result("Indeterminate result");
        }
    }

    // stream inserter
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const interval<T>& intvl) {
        os << "[" << intvl.lower() << "," << intvl.upper() << "]";
        return os;
    }

}  // namespace ra::math