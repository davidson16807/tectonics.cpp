
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <series/series.hpp>  
#include <series/arithmetic.hpp>  
#include <series/relational.hpp>  
#include <series/convenience.hpp>  
#include <series/operators.hpp>  

#include "geometric.hpp"
#include "relational.hpp"


TEST_CASE( "Series<T> dot() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

    SECTION("dot() must be called repeatedly without changing the output"){
        dot(a, b, c1);
        dot(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T> dot() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     series::vec3s b = series::get_random_vec3s(5, generator);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T> dot() commutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ba = series::floats({0,0,0,0,0});

    SECTION("dot(a,b) must equal dot(b,a)"){
        dot(a, b, ab);
        dot(a, b, ba);
        CHECK(ab==ba);
    }
}

TEST_CASE( "Series<T> dot() associativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::floats c = series::floats({0,1,2,3,4});
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::floats ab_c = series::floats({0,0,0,0,0});
    series::floats a_bc = series::floats({0,0,0,0,0});

    SECTION("dot(dot(a,b),c) must equal dot(a,dot(b,c))"){
        dot(a, b, ab_c);
        mult(ab_c, c, ab_c);
        mult(b, c, bc);
        dot(a, bc, a_bc);
        CHECK(ab_c==a_bc);
    }
}

TEST_CASE( "Series<T> dot() distributivity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::floats ac = series::floats({0,0,0,0,0});
    series::floats bc = series::floats({0,0,0,0,0});
    series::floats ab_c = series::floats({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        dot(a+b, c, ab_c);
        dot(a, c, ac);
        dot(b, c, bc);
        CHECK(ab_c==ac+bc);
    }
}






TEST_CASE( "Series<T>/singleton dot() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

    SECTION("dot() must be called repeatedly without changing the output"){
        dot(a, b, c1);
        dot(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T>/singleton dot() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     glm::vec3 b(-1,0.5,2);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T>/singleton dot() commutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ba = series::floats({0,0,0,0,0});

    SECTION("dot(a,b) must equal dot(b,a)"){
        dot(a, b, ab);
        dot(a, b, ba);
        CHECK(ab==ba);
    }
}

TEST_CASE( "Series<T>/singleton dot() associativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::floats c = series::floats({0,1,2,3,4});
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::floats ab_c = series::floats({0,0,0,0,0});
    series::floats a_bc = series::floats({0,0,0,0,0});

    SECTION("dot(dot(a,b),c) must equal dot(a,dot(b,c))"){
        dot(a, b, ab_c);
        mult(ab_c, c, ab_c);
        mult(b, c, bc);
        dot(a, bc, a_bc);
        CHECK(ab_c==a_bc);
    }
}

TEST_CASE( "Series<T>/singleton dot() distributivity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::floats ac = series::floats({0,0,0,0,0});
    series::floats bc = series::floats({0,0,0,0,0});
    series::floats ab_c = series::floats({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        dot(a+b, c, ab_c);
        dot(a, c, ac);
        dot(b, c, bc);
        CHECK(ab_c==ac+bc);
    }
}


















TEST_CASE( "Series<T> cross() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s c1 = series::get_random_vec3s(5, generator);
    series::vec3s c2 = series::get_random_vec3s(5, generator);

    SECTION("cross() must be called repeatedly without changing the output"){
        cross(a, b, c1);
        cross(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T> cross() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     series::vec3s b = series::get_random_vec3s(5, generator);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T> cross() anticommutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s ab = series::get_random_vec3s(5, generator);
    series::vec3s ba = series::get_random_vec3s(5, generator);

    SECTION("cross(a,b) must equal -cross(b,a)"){
        cross(a, b, ab);
        cross(b, a, ba);
        CHECK(ab==-ba);
    }
}

TEST_CASE( "Series<T> cross() associativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s c = series::vec3s({0,1,2,3,4});
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::vec3s ab_c = series::get_random_vec3s(5, generator);
    series::vec3s a_bc = series::get_random_vec3s(5, generator);

    SECTION("dot(dot(a,b),c) must equal dot(a,dot(b,c))"){
        cross(a, b, ab_c);
        mult(ab_c, c, ab_c);
        mult(b, c, bc);
        cross(a, bc, a_bc);
        CHECK(ab_c==a_bc);
    }
}

TEST_CASE( "Series<T> cross() distributivity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::vec3s ac = series::get_random_vec3s(5, generator);
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::vec3s ab_c = series::get_random_vec3s(5, generator);

    SECTION("a+b must equal b+a"){
        cross(a+b, c, ab_c);
        cross(a, c, ac);
        cross(b, c, bc);
        CHECK(ab_c==ac+bc);
    }
}






TEST_CASE( "Series<T>/singleton cross() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s c1 = series::get_random_vec3s(5, generator);
    series::vec3s c2 = series::get_random_vec3s(5, generator);

    SECTION("cross() must be called repeatedly without changing the output"){
        cross(a, b, c1);
        cross(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T>/singleton cross() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     glm::vec3 b(-1,0.5,2);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T>/singleton cross() anticommutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s ab = series::get_random_vec3s(5, generator);
    series::vec3s ba = series::get_random_vec3s(5, generator);

    SECTION("cross(a,b) must equal -cross(b,a)"){
        cross(a, b, ab);
        cross(b, a, ba);
        CHECK(ab==-ba);
    }
}

TEST_CASE( "Series<T>/singleton cross() associativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s c = series::vec3s({0,1,2,3,4});
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::vec3s ab_c = series::get_random_vec3s(5, generator);
    series::vec3s a_bc = series::get_random_vec3s(5, generator);

    SECTION("cross(cross(a,b),c) must equal cross(a,cross(b,c))"){
        cross(a, b, ab_c);
        mult(ab_c, c, ab_c);
        mult(b, c, bc);
        cross(a, bc, a_bc);
        CHECK(ab_c==a_bc);
    }
}

TEST_CASE( "Series<T>/singleton cross() distributivity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::vec3s ac = series::get_random_vec3s(5, generator);
    series::vec3s bc = series::get_random_vec3s(5, generator);
    series::vec3s ab_c = series::get_random_vec3s(5, generator);

    SECTION("a+b must equal b+a"){
        cross(a+b, c, ab_c);
        cross(a, c, ac);
        cross(b, c, bc);
        CHECK(ab_c==ac+bc);
    }
}


























TEST_CASE( "Series<T> distance() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

    SECTION("distance() must be called repeatedly without changing the output"){
        distance(a, b, c1);
        distance(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T> distance() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     series::vec3s b = series::get_random_vec3s(5, generator);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T> distance() commutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ba = series::floats({0,0,0,0,0});

    SECTION("distance(a,b) must equal distance(b,a)"){
        distance(a, b, ab);
        distance(a, b, ba);
        CHECK(ab==ba);
    }
}

// TEST_CASE( "Series<T> distance() associativity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     series::vec3s b = series::get_random_vec3s(5, generator);
//     series::floats c = series::floats({0,1,2,3,4});
//     series::vec3s bc = series::get_random_vec3s(5, generator);
//     series::floats ab_c = series::floats({0,0,0,0,0});
//     series::floats a_bc = series::floats({0,0,0,0,0});

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         distance(a, b, ab_c);
//         mult(ab_c, c, ab_c);
//         mult(b, c, bc);
//         distance(a, bc, a_bc);
//         CHECK(ab_c==a_bc);
//     }
// }

TEST_CASE( "Series<T> distance() translational invariance", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ac_bc = series::floats({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        distance(a+c, b+c, ac_bc);
        distance(a, b, ab);
        CHECK(ac_bc==ab);
    }
}






TEST_CASE( "Series<T>/singleton distance() purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

    SECTION("distance() must be called repeatedly without changing the output"){
        distance(a, b, c1);
        distance(a, b, c2);
        CHECK(c1==c2);
    }
}

// TEST_CASE( "Series<T>/singleton distance() identity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     glm::vec3 b(-1,0.5,2);
//     series::vec3s ab = series::get_random_vec3s(5, generator);
//     series::vec3s ba = series::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(a+zeros==a);
//         CHECK(a-zeros==a);
//         CHECK(a*ones ==a);
//         CHECK(a/ones ==a);
//         CHECK(a-a==zeros);
//         CHECK(a/a==ones );
//     }
// }

TEST_CASE( "Series<T>/singleton distance() commutativity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ba = series::floats({0,0,0,0,0});

    SECTION("distance(a,b) must equal distance(b,a)"){
        distance(a, b, ab);
        distance(a, b, ba);
        CHECK(ab==ba);
    }
}

// TEST_CASE( "Series<T>/singleton distance() associativity", "[many]" ) {
//     std::mt19937 generator(2);
//     series::vec3s a = series::get_random_vec3s(5, generator);
//     glm::vec3 b(-1,0.5,2);
//     series::floats c = series::floats({0,1,2,3,4});
//     series::vec3s bc = series::get_random_vec3s(5, generator);
//     series::floats ab_c = series::floats({0,0,0,0,0});
//     series::floats a_bc = series::floats({0,0,0,0,0});

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         distance(a, b, ab_c);
//         mult(ab_c, c, ab_c);
//         mult(b, c, bc);
//         distance(a, bc, a_bc);
//         CHECK(ab_c==a_bc);
//     }
// }

TEST_CASE( "Series<T>/singleton distance() translational invariance", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    glm::vec3 b(-1,0.5,2);
    series::vec3s c = series::get_random_vec3s(5, generator);
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats ac_bc = series::floats({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        distance(a+c, b+c, ac_bc);
        distance(a, b, ab);
        CHECK(ac_bc==ab);
    }
}







TEST_CASE( "Series<T> length() scale invariance", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);

    SECTION("length(a*b) must equal length(a)*b for b>=0"){
        CHECK(length(a*5.0f)==length(a)*5.0f);
    }
}