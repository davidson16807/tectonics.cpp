#pragma once

// C libraries

// in-house libraries

namespace aggregated
{

	template <typename Op>
	class LeftBinary
	{
		const Op op;
	public:
		LeftBinary(const Op& op):
			op(op)
		{}
		LeftBinary():
			op()
		{}

		template <typename T>
		auto operator() (const T& a) const
		{
			auto out = op.identity;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = op(out, a[i]);
			}
			return out;
		}

		// // comp

	};

	template <typename Op>
	class RightBinary
	{
		const Op op;
	public:
		RightBinary(const Op& op):
			op(op)
		{}
		RightBinary():
			op()
		{}

		template <typename T>
		auto operator() (const T& a) const
		{
			auto out = op.identity;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				out = op(a[i], out);
			}
			return out;
		}

	};

}

