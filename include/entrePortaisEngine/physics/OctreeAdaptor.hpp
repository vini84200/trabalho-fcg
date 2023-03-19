#ifndef ENTREPORTAIS_OCTREEADAPTOR_HPP
#define ENTREPORTAIS_OCTREEADAPTOR_HPP


#include "Boxes.hpp"
#include "octree.h"
#include <octree_container.h>

namespace entre_portais {

    struct BoxAdaptorBasics {
        // FONTE: https://github.com/attcs/Octree
        static inline float &point_comp(glm::vec3 &p, OrthoTree::dim_type iDimension) {
            switch (iDimension) {
                case 0:
                    return p.x;
                case 1:
                    return p.y;
                case 2:
                    return p.z;
                default:
                    assert(false);
                    return p.x;
            }
        }

        static constexpr float point_comp_c(glm::vec3 const &p, OrthoTree::dim_type iDimension) {
            switch (iDimension) {
                case 0:
                    return p.x;
                case 1:
                    return p.y;
                case 2:
                    return p.z;
                default:
                    assert(false);
                    return p.x;
            }
        }

        static inline glm::vec3 &box_min(BoundingBox &b) {
            return b.min;
        }

        static inline glm::vec3 &box_max(BoundingBox &b) {
            return b.max;
        }

        static inline glm::vec3 const &box_min_c(BoundingBox const &b) {
            return b.min;
        }

        static inline glm::vec3 const &box_max_c(BoundingBox const &b) {
            return b.max;
        }
    };

    using OctreeAdaptor = OrthoTree::AdaptorGeneralBase<3, glm::vec3, BoundingBox, BoxAdaptorBasics, float>;

    using OctreeBB = OrthoTree::OrthoTreeBoundingBox<3, glm::vec3, BoundingBox, OctreeAdaptor, float>;
    using OctreeBBC = OrthoTree::OrthoTreeContainerBox<OctreeBB, BoundingBox>;
} // entre_portais

#endif //ENTREPORTAIS_OCTREEADAPTOR_HPP
