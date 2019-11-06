	uint get_vertex_tree_id(
		const vec3 x, 
		const uint N, 
		const vec3s& face_midpoints, 
		const uints& face_vertex_ids
	) {
		uint id = std::min_element(
			face_midpoints.begin(), 
			face_midpoints.end(), 
			[x](std::pair<int, glm::vec3> a, std::pair<int, glm::vec3> b) -> bool {
				return glm::distance(a.second, x) < glm::distance(b.second, x);
			}
		) - face_midpoints.begin();
		uvec3 face = face_vertex_ids[id];
		vec3 a = points[face.x];
		vec3 b = points[face.y];
		vec3 c = points[face.z];
		for (int n = 0; n < N; ++n)
		{
			id *= 3;
			a_dx = glm::distance(a,x);
			b_dx = glm::distance(b,x);
			c_dx = glm::distance(c,x);
			min_dx = min(a_dx, min(b_dx, c_dx));
			if (a_dx == min_dx)
			{
				b = glm::normalize((a+b)/2.f);
				c = glm::normalize((a+c)/2.f);
				id += 0;
			} 
			else if (b_dx == min_dx)
			{
				a = glm::normalize((b+a)/2.f);
				c = glm::normalize((b+c)/2.f);
				id += 1;
			}
			else if (c_dx == min_dx)
			{
				a = glm::normalize((c+a)/2.f);
				b = glm::normalize((c+b)/2.f);
				id += 2;
			}
		}
		return id;
	}

	std::vector<uint> get_vertex_id_map(
		const uint N, 
		const std::unordered_map<glm::uvec2, uint>& midpoints
	) {
		std::vector<uint> vertex_id_map(12*pow(3,N+1), -1);
		for (int i = 12*pow(3,N); i < 12*pow(3,N+1); ++i)
		{
			vertex_id_map[i] = get_vertex_id(i, N, midpoints);
		}
		return vertex_id_map;
	}

	uint get_vertex_id(
		const uint vertex_tree_id, 
		const uint N, 
		const std::unordered_map<glm::uvec2, uint>& midpoints
	) {
		float i = vertex_tree_id / pow(3.f,N);
		uint i_n = trunc(i);
		uvec3 face = face_vertex_ids[i_n];
		uint a = face.x;
		uint b = face.y;
		uint c = face.z;
		for (int n = 0; n < N; ++n)
		{
			i -= i_n;
			i *= 3;
			i_n = trunc(i);
			if (i_n == 0)
			{
				b = midpoints[glm::uvec2(a, b)];
				c = midpoints[glm::uvec2(a, c)];
			} 
			else if (i_n == 1)
			{
				a = midpoints[glm::uvec2(b, a)];
				c = midpoints[glm::uvec2(b, c)];
			}
			else if (i_n == 2)
			{
				a = midpoints[glm::uvec2(c, a)];
				b = midpoints[glm::uvec2(c, b)];
			}
		}
		if      (i_n == 0) { return a; } 
		else if (i_n == 1) { return b; } 
		else if (i_n == 2) { return c; } 
	}


// considerations:
// in-order/out-of-order memory access
// size of memory footprint
// frequency of access