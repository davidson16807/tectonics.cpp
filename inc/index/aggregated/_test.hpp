

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <index/procedural/Uniform.hpp>
#include <index/adapted/scalar/ScalarClosedForm.hpp>
#include <index/adapted/symbolic/SymbolicEquivalence.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/iterated/ClosedForm.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Order.hpp>
#include <index/whole.hpp>
#include "Equivalence.hpp"

TEST_CASE( "Series<T> abs determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({-1,1,-2,2,3});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("abs(a) must generate the same output when called repeatedly"){
        closed_form.abs(a, out1);
        closed_form.abs(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> sign determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({-1,1,-2,2,3});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("sign(a) must generate the same output when called repeatedly"){
        closed_form.sign(a, out1);
        closed_form.sign(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> abs/sign consistency", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Arithmetic<adapted::SymbolicArithmetic> arithmetic;
    auto a = std::vector({-1,1,-2,2,3});
    auto abs_a = std::vector({0,0,0,0,0});
    auto sign_a = std::vector({0,0,0,0,0});
    auto a2 = std::vector({0,0,0,0,0});
    SECTION("sign(a)*abs(a) must reconstruct a"){
        closed_form.abs(a, abs_a);
        closed_form.sign(a, sign_a);
        arithmetic.multiply(sign_a, abs_a, a2);
        CHECK(equivalence.equal(a2, a));
    }
}
TEST_CASE( "Series<T> floor determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({-1,1,-2,2,3});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("floor(a) must generate the same output when called repeatedly"){
        closed_form.floor(a, out1);
        closed_form.floor(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> fract determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({-1,1,-2,2,3});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("fract(a) must generate the same output when called repeatedly"){
        closed_form.fract(a, out1);
        closed_form.fract(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> floor/fract consistency", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Arithmetic<adapted::SymbolicArithmetic> arithmetic;
    auto a = std::vector({-1,1,-2,2,3});
    auto floor_a = std::vector({0,0,0,0,0});
    auto fract_a = std::vector({0,0,0,0,0});
    auto a2 = std::vector({0,0,0,0,0});
    SECTION("fract(a)+floor(a) must reconstruct a"){
        closed_form.floor(a, floor_a);
        closed_form.fract(a, fract_a);
        arithmetic.add(floor_a, fract_a, a2);
        CHECK(equivalence.equal(a2, a));
    }
}

TEST_CASE( "Series<T> residue determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({-1,1,-2,2,3});
    auto b = std::vector({1,2,3,4,5});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("residue(a,b) must generate the same output when called repeatedly"){
        closed_form.residue(a,b, out1);
        closed_form.residue(a,b, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> residue/fract consistency", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    auto a = std::vector({1.0,2.1,3.1,4.2,5.3});
    auto ones = std::vector({1,1,1,1,1});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("residue(a,1) must generate the same output as fract(a) for positive numbers"){
        closed_form.residue(a, procedural::uniform(1), out1);
        closed_form.fract(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
    SECTION("residue(a,ones) must generate the same output as fract(a) for positive numbers"){
        closed_form.residue(a,ones, out1);
        closed_form.fract(a, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("min(a,b) must generate the same output when called repeatedly"){
        order.min(a,b, out1);
        order.min(a,b, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min decreasing", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto c = std::vector({10,0,0,0,-10});
    auto ab = std::vector({0,0,0,0,0});
    auto abc = std::vector({0,0,0,0,0});
    SECTION("sum(min(a,b)) must always return a value less than or equal to the starting value"){
        order.min(a,b, ab);
        order.min(ab,c, abc);
        CHECK(whole::sum<int>(ab) >= whole::sum<int>(abc));
    }
}
TEST_CASE( "Series<T> min idempotence", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto ab = std::vector({0,0,0,0,0});
    auto abb = std::vector({0,0,0,0,0});
    SECTION("subsequent calls to min(*,b) must return the same value"){
        order.min(a,b, ab);
        order.min(ab,b, abb);
        CHECK(equivalence.equal(ab, abb));
    }
}

TEST_CASE( "Series<T> max determinism", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto out1 = std::vector({0,0,0,0,0});
    auto out2 = std::vector({0,0,0,0,0});
    SECTION("max(a,b) must generate the same output when called repeatedly"){
        order.max(a,b, out1);
        order.max(a,b, out2);
        CHECK(equivalence.equal(out1, out2));
    }
}
TEST_CASE( "Series<T> max decreasing", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto c = std::vector({10,0,0,0,-10});
    auto ab = std::vector({0,0,0,0,0});
    auto abc = std::vector({0,0,0,0,0});
    SECTION("sum(max(a,b)) must always return a value greater than or equal to the starting value"){
        order.max(a,b, ab);
        order.max(ab,c, abc);
        CHECK(whole::sum<int>(ab) <= whole::sum<int>(abc));
    }
}
TEST_CASE( "Series<T> max idempotence", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({3,2,-2,1,-1});
    auto b = std::vector({1,2,3,4,5});
    auto ab = std::vector({0,0,0,0,0});
    auto abb = std::vector({0,0,0,0,0});
    SECTION("subsequent calls to max(*,b) must return the same value"){
        order.max(a,b, ab);
        order.max(ab,b, abb);
        CHECK(equivalence.equal(ab, abb));
    }
}

TEST_CASE( "Series<T> min/max/clamp consistency", "[aggregated]" ) {
    aggregated::Equivalence<adapted::SymbolicEquivalence> equivalence;
    iterated::ClosedForm<adapted::ScalarClosedForm<int>> closed_form;
    iterated::Order<adapted::SymbolicOrder> order;
    auto a = std::vector({1,2,3,4,5});
    auto lo = std::vector({-1,1,-2,2,3});
    auto hi = std::vector({7,6,5,4,3});
    auto alo = std::vector({0,0,0,0,0});
    auto alohi1 = std::vector({0,0,0,0,0});
    auto alohi2 = std::vector({0,0,0,0,0});
    SECTION("min(max(a,lo),hi) must return the same value as clamp(a,lo,hi)"){
        order.max(a,lo, alo);
        order.min(alo,hi, alohi1);
        order.clamp(a,lo,hi, alohi2);
        CHECK(equivalence.equal(alohi1, alohi2));
    }
}





// TEST_CASE( "Series<T> string cast determinism", "[aggregated]" ) {
//     auto a = std::vector({1,2,3,4,5});
//     SECTION("to_string(a) must be called repeatedly without changing the output"){
//         CHECK(whole::to_string(a) == whole::to_string(a));
//     }
// }

// TEST_CASE( "Series<T> string cast correctness", "[aggregated]" ) {
//     auto a = std::vector({1,2,3,4,5,6});
//     auto b = std::vector({1,1,2,3,5,8});
//     auto c = std::vector<float>({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
//                            4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
//                            1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
//                            4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
//                            4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
//                            4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
//                            1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
//                            4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});
//     std::string stra = whole::to_string(a);
//     std::string strb = whole::to_string(b);
//     std::string strc = whole::to_string(c);
//     SECTION("to_string() must render correct representation"){
//         REQUIRE_THAT(stra, Catch::Contains("█"));
//         REQUIRE_THAT(strb, Catch::Contains("█"));
//         REQUIRE_THAT(whole::to_string(std::vector({INFINITY})), Catch::Contains("∞"));
//         REQUIRE_THAT(whole::to_string(std::vector({std::sqrt(-1)})), Catch::Contains("N"));
//     }
// }

// TEST_CASE( "Series<T> string cast representation", "[aggregated]" ) {
//     auto a = std::vector({1,2,3,4,5,6});
//     SECTION("to_string() must capture appreciable differences in the content of Series<T>"){
//         for(std::size_t i=0; i<a.size(); ++i){
//             auto b = std::vector({1,2,3,4,5,6});
//             b[i] = -1;
//             std::string stra = whole::to_string(a);
//             std::string strb = whole::to_string(b);
//             CHECK(stra != strb);
//         }
//     }
//     SECTION("to_string() must capture appreciable differences in the length of Series<T>"){
//         auto c = std::vector({1,2,3,4,5,6,7});
//         std::string stra = whole::to_string(a);
//         std::string strc = whole::to_string(c);
//         CHECK(stra != strc);
//     }
// }
