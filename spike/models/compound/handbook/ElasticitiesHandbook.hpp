#pragma once

// in-house libraries
#include <models/compound/correlation/elasticity.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    using SolidBulkModulusTemperatureRelation = published::SolidBulkModulusTemperatureRelation;
    using SolidTensileModulusTemperatureRelation = published::SolidTensileModulusTemperatureRelation;
    using SolidLameParameterTemperatureRelation = published::SolidLameParameterTemperatureRelation;
    using SolidShearModulusTemperatureRelation = published::SolidShearModulusTemperatureRelation;
    using SolidPoissonRatioTemperatureRelation = published::SolidPoissonRatioTemperatureRelation;
    using SolidPwaveModulusTemperatureRelation = published::SolidPwaveModulusTemperatureRelation;

    struct ElasticitiesHandbook
    {
        table::FullTable<SolidBulkModulusTemperatureRelation> bulk_modulus;
        table::FullTable<SolidTensileModulusTemperatureRelation> tensile_modulus;
        table::FullTable<SolidLameParameterTemperatureRelation> lame_parameter;
        table::FullTable<SolidShearModulusTemperatureRelation> shear_modulus;
        table::FullTable<SolidPoissonRatioTemperatureRelation> poisson_ratio;
        table::FullTable<SolidPwaveModulusTemperatureRelation> pwave_modulus;

        ElasticitiesHandbook(
            const table::PartialTable<SolidBulkModulusTemperatureRelation> K,
            const table::PartialTable<SolidTensileModulusTemperatureRelation> E,
            const table::PartialTable<SolidLameParameterTemperatureRelation> l,
            const table::PartialTable<SolidShearModulusTemperatureRelation> G,
            const table::PartialTable<SolidPoissonRatioTemperatureRelation> nu,
            const table::PartialTable<SolidPwaveModulusTemperatureRelation> M,
            const table::FullTable<int> similarity,
            const int fallback_id
        ){

            // Standardize on bulk and tensile modulus since they seem to be reported most often,
            // then use those two to calculate the remaining modulii.

            using Modulus = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
            using Poisson = relation::GenericRelation<si::temperature<double>,double>;

            auto E2 = table::first<Modulus>({ E,
                table::gather<Modulus>([](auto K,  auto G ) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_shear   (K(T), G(T)); }); }, K, G ),
                table::gather<Modulus>([](auto K,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_poisson (K(T), nu(T));}); }, K, nu),
                table::gather<Modulus>([](auto K,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_bulk_and_pwave   (K(T), M(T)); }); }, K, M ),
                table::gather<Modulus>([](auto l,  auto G ) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_shear   (l(T), G(T)); }); }, l, G ),
                table::gather<Modulus>([](auto l,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_poisson (l(T), nu(T));}); }, l, nu),
                table::gather<Modulus>([](auto l,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_lame_and_pwave   (l(T), M(T)); }); }, l, M ),
                table::gather<Modulus>([](auto G,  auto nu) { return Modulus([=](auto T){ return correlation::get_tensile_from_shear_and_poisson(G(T), nu(T));}); }, G, nu),
                table::gather<Modulus>([](auto G,  auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_shear_and_pwave  (G(T), M(T)); }); }, G, M ),
                table::gather<Modulus>([](auto nu, auto M ) { return Modulus([=](auto T){ return correlation::get_tensile_from_poisson_and_pwave(nu(T),M(T)); }); }, nu,M ),
            });

            auto K2 = table::first<Modulus>({ K,
                table::gather<Modulus>([](auto E,  auto l ) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_lame   (E(T), l(T) ); }); }, E,  l ),
                table::gather<Modulus>([](auto E,  auto G ) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_shear  (E(T), G(T) ); }); }, E,  G ),
                table::gather<Modulus>([](auto E,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_tensile_and_poisson(E(T), nu(T)); }); }, E,  nu),
                table::gather<Modulus>([](auto l,  auto G ) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_shear     (l(T), G(T) ); }); }, l,  G ),
                table::gather<Modulus>([](auto l,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_poisson   (l(T), nu(T)); }); }, l,  nu),
                table::gather<Modulus>([](auto l,  auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_lame_and_pwave     (l(T), M(T) ); }); }, l,  M ),
                table::gather<Modulus>([](auto G,  auto nu) { return Modulus([=](auto T){ return correlation::get_bulk_from_shear_and_poisson  (G(T), nu(T)); }); }, G,  nu),
                table::gather<Modulus>([](auto G,  auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_shear_and_pwave    (G(T), M(T) ); }); }, G,  M ),
                table::gather<Modulus>([](auto nu, auto M ) { return Modulus([=](auto T){ return correlation::get_bulk_from_poisson_and_pwave  (nu(T), M(T)); }); }, nu, M ),
            });

            auto l2  = table::first<Modulus>({ l,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_lame_from_bulk_and_tensile    (K(T),E(T)); }); }, K2, E2), });
            auto G2  = table::first<Modulus>({ G,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_shear_from_bulk_and_tensile   (K(T),E(T)); }); }, K2, E2), });
            auto nu2 = table::first<Poisson>({ nu, table::gather<Poisson>([](auto K, auto E) { return Poisson([=](auto T){ return correlation::get_poisson_from_bulk_and_tensile (K(T),E(T)); }); }, K2, E2), });
            auto M2  = table::first<Modulus>({ M,  table::gather<Modulus>([](auto K, auto E) { return Modulus([=](auto T){ return correlation::get_pwave_from_bulk_and_tensile   (K(T),E(T)); }); }, K2, E2), });

            bulk_modulus    = table::complete(table::imitate(K2,  similarity), K2 [fallback_id], similarity.size());
            tensile_modulus = table::complete(table::imitate(E2,  similarity), E2 [fallback_id], similarity.size());
            lame_parameter  = table::complete(table::imitate(l2,  similarity), l2 [fallback_id], similarity.size());
            shear_modulus   = table::complete(table::imitate(G2,  similarity), G2 [fallback_id], similarity.size());
            poisson_ratio   = table::complete(table::imitate(nu2, similarity), nu2[fallback_id], similarity.size());
            pwave_modulus   = table::complete(table::imitate(M2,  similarity), M2 [fallback_id], similarity.size());

        }
    };

}}

