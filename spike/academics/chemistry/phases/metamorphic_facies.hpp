
namespace phase
{
    enum MetamorphicFacies
    {
        igneous_or_sediment,  
        sedimentary,          
        zeolite,              
        magmatism,            
        blueschist,           
        eclogite,             
        hornfels,             
        prehnite_pumpellyte,  
        greenschist,          
        epidote_amphibiolite, 
        amphibiolite,         
        granulite,            
        COUNT
    }
    /*
    `get_metamorphic_facies` returns the facies of a rock that has been submitted to a given temperature and pressure.

    Metamorphic facies are like phases of matter for rocks.
    Rocks are complex assortments of mineral grains. 
    Each mineral has its own phase diagram, 
    but there are many kinds of minerals and their phase diagrams are poorly documented. 
    However the phase diagrams for minerals share some consistencies,
    and these consistencies allow us to make predictions about what an earth-like rock will look like
    when submitted to a given pressure and temperature.
    This will apply to any rock that is composed of minerals commonly found on earth.

    Fascies are represented as spherical regions on a PT diagram for both performance and ease of modeling.
    We check for intersection with them in a given order, and the first match determines the fascies.
    See research/lithosphere/facies.gif for the source diagram that we use derived these numbers.
    We also use this diagram as reason to set an equivalence of 900K = 20kb 
    for determining less well known forms of metamorphism and lithification.
    */
    MetamorphicFacies get_metamorphic_facies(float p, float t)
    {
        glm::vec2 pt_scaled = glm::vec2(p,t) / glm::vec2(2e8*units::pascal, 100*units::kelvin);

        if ( glm::length(pt_scaled-glm::vec2(0,2.65))  < 0.011 )
        {
            return MetamorphicFacies::igneous_or_sediment;
        }
        else if ( glm::length(pt_scaled-glm::vec2(0,2.65))  < 2.0 )
        {
            return MetamorphicFacies::sedimentary;
        }
        else if( glm::length(pt_scaled-glm::vec2(0,2.65))  < 3.0 )
        {
            return MetamorphicFacies::zeolite;
        }
        else if( glm::length(pt_scaled-glm::vec2(9.1,-4.8))  < 11.5 )
        {
            return MetamorphicFacies::blueschist;
        }
        else if( glm::length(pt_scaled-glm::vec2(13.5,7.1))  < 7.4 )
        {
            return MetamorphicFacies::eclogite;
        }
        else if( glm::length(pt_scaled-glm::vec2(-9.2,7.6))  < 10.5 )
        {
            return MetamorphicFacies::hornfels;
        }
        else if( glm::length(pt_scaled-glm::vec2(0.9,-9.2))  < 15.4 )
        {
            return MetamorphicFacies::prehnite_pumpellyte;
        }
        else if( glm::length(pt_scaled-glm::vec2(5.9,-8.15))  < 15.6 )
        {
            return MetamorphicFacies::greenschist;
        }
        else if( glm::length(pt_scaled-glm::vec2(6.6, -11.8))  < 19.8 )
        {
            return MetamorphicFacies::epidote_amphibiolite;
        }
        else if( glm::length(pt_scaled-glm::vec2(6.1,-0.75))  < 10.5 )
        {
            return MetamorphicFacies::amphibolite;
        }
        else
        {
            return MetamorphicFacies::granulite;
        }
    }
}
    