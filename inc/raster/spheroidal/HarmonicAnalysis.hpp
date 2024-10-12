#pragma once

#include <index/procedural/Map.hpp>

#include <field/SphericalHarmonics.hpp>

namespace spheroidal 
{

	/*
	`spheroidal::HarmonicAnalysis<T,Lhi>` represents maps between 
	harmonic and spatial domains on a spheroidal object using spherical harmonics.
	It presumes the existance of `field::SphericalHarmonics<T,Lhi>` in order to represent the harmonic domain
	and represents the spatial domain using 1d arrays of scalar values, cell positions, are cell areas
	of which cell properties are ideally specified procedurally for performance reasons.
	*/
	template <typename T, int Lhi, typename Positions, typename Areas>
	class HarmonicAnalysis
	{
        static constexpr int coefficient_count = (Lhi+1)*(Lhi+1);

        constexpr int coefficient_id(const int M, const int L) const
        {
            return (L-1)*(L-1) + M+L;
        }

		const Positions positions;
		const Areas areas;
		T total_area;

		template<int M, int L>
		void decompose_orders(
			const T scalar,
			const T z,
			const T phi,
			const T fractional_area,
			std::array<T,coefficient_count>& inout
		) const {
			if constexpr(M>=-L)
			{
				decompose_orders<M-1,L>(scalar, z, phi, fractional_area, inout);
				inout[coefficient_id(M,L)] += 
					scalar * 
					field::SphericalHarmonic<T,M,L>().call(z, phi) * 
					fractional_area;
			}
		}

		template<int L>
		void decompose_degrees(
			const T scalar,
			const T z,
			const T phi,
			const T fractional_area,
			std::array<T,coefficient_count>& inout
		) const {
			if constexpr(L>=0)
			{
				// decompose_degrees<L-1>(scalar, z, phi, fractional_area, inout);
				decompose_orders<L,L>(scalar, z, phi, fractional_area, inout);
			}
		}

	public:
		HarmonicAnalysis(
			const Positions& positions,
			const Areas& areas
		):
			positions(positions),
			areas(areas)
		{
			total_area = T(0);
			for (std::size_t i = 0; i < areas.size(); ++i)
			{
				total_area += areas[i];
			}
		}

		template<typename Raster>
		field::SphericalHarmonics<T,Lhi> decompose(
			const Raster& raster
		) const {
			std::array<T,coefficient_count> coefficients;
			coefficients.fill(T(0));
			for (std::size_t i = 0; i < raster.size(); ++i)
			{
				decompose_degrees<Lhi>(
					raster[i],
					glm::normalize(positions[i]).z, 
					std::atan2(positions[i].y, positions[i].x),
					areas[i] / total_area,
					coefficients
				);
			}
			return field::SphericalHarmonics<T,Lhi>(coefficients);
		}

		auto compose(
			const field::SphericalHarmonics<T,Lhi>& harmonics
		) const {
			return procedural::map(harmonics, positions);
		}

	};

	template <typename T, int Lhi, typename Positions, typename Areas>
	auto harmonic_analysis(
		const Positions& positions,
		const Areas& areas
	) {
		return HarmonicAnalysis<T,Lhi,Positions,Areas>(positions, areas);
	}

}

