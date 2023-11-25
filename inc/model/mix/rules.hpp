#pragma once

// in house libraries
#include <model/compound/relation/relation.hpp>

namespace mix{

    double linear_rule(const std::vector<double>& relations, const std::vector<double>& fractions)
    {
        double result(0);
        for (std::size_t i=0; i<relations.size(); i++){
            result += fractions[i] * relations[i];
        }
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    compound::relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi> linear_rule(const std::vector<compound::relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi>>& relations, const std::vector<double>& fractions)
    {
        compound::relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result;
        for (std::size_t i=0; i<relations.size(); i++){
            result += fractions[i] * relations[i];
        }
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    compound::relation::GenericRelation<Tx, Ty> linear_rule(
        const std::vector<compound::relation::ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>>& relations, 
        const std::vector<double>& fractions
    ){
        return compound::relation::GenericRelation<Tx, Ty>(
            [=](Tx x){
                auto result = fractions[0] * relations[0](x);
                for (std::size_t i=1; i<fractions.size(); i++) {
                    result += fractions[i] * relations[i](x);
                }
                return result;
            }
        );
    }

    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    compound::relation::RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> linear_rule(
        const std::vector<compound::relation::RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi>>& relations, 
        const std::vector<double>& fractions
    ){
        compound::relation::RationalRailyardRelation<Tx,Ty,Plo,Phi,Qlo,Qhi> result(0);
        for (std::size_t i=0; i<relations.size(); i++){
            result = result + fractions[i] * relations[i];
        }
        return result;
    }

    template<typename Ty>
    compound::relation::GasPropertyStateRelation<Ty> linear_rule(const std::vector<compound::relation::GasPropertyStateRelation<Ty>>& relations, const std::vector<double>& fractions)
    {
        compound::relation::GasPropertyStateRelation<Ty> result;
        for (std::size_t i=0; i<relations.size(); i++){
            result += fractions[i] * relations[i];
        }
        return result;
    }

    template<typename Tx, typename Ty>
    compound::relation::GenericRelation<Tx, Ty> linear_rule(const std::vector<compound::relation::GenericRelation<Tx, Ty>>& relations, const std::vector<double>& fractions)
    {
        return compound::relation::GenericRelation<Tx, Ty>(
            [=](Tx x){
                auto result = fractions[0] * relations[0](x);
                for (std::size_t i=1; i<fractions.size(); i++) {
                    result += fractions[i] * relations[i](x);
                }
                return result;
            }
        );
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
    si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> linear_rule(const std::vector<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& relations, const std::vector<double>& fractions)
    {
        si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> result;
        for (std::size_t i=0; i<relations.size(); i++){
            result += fractions[i] * relations[i];
        }
        return result;
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
    si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> parallel_rule(const std::vector<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>>& relations, const std::vector<double>& fractions)
    {
        si::units<-M1,-KG1,-S1,-K1,-MOL1,-A1,-CD1,T1> result;
        for (std::size_t i=0; i<relations.size(); i++){
            result += fractions[i] / relations[i];
        }
        return 1.0 / result;
    }

    // volume weighted parallel mean
    template<typename Tx, typename Ty>
    compound::relation::GenericRelation<Tx, Ty> parallel_rule(const std::vector<compound::relation::GenericRelation<Tx, Ty>>& relations, const std::vector<double>& fractions){
        return compound::relation::GenericRelation<Tx, Ty>(
            [=](Tx x){
                auto result = fractions[0] / relations[0](x);
                for (std::size_t i=1; i<fractions.size(); i++) {
                    result += fractions[i] / relations[i](x);
                }
                return 1.0 / result;
            }
        );
    }

}

