#pragma once

template<typename id, typename scalar>
struct TrackPosition {

    using vec3 = glm::vec<3,scalar,glm::defaultp>;

    const id   node;
    const vec3 position;

    TrackPosition(
        const id node, 
        const vec3& position
    ): 
        node(node), 
        position(position) 
    {}

};
