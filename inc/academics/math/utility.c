/*
This function returns a rgb vector that quickly approximates a spectral "bump".
Adapted from GPU Gems and Alan Zucconi
from https://www.alanzucconi.com/2017/07/15/improving-the-rainbow/
*/
float bump (
    in float x, 
    in float edge0, 
    in float edge1, 
    in float height
){
    float center = (edge1 + edge0) / 2.;
    float width  = (edge1 - edge0) / 2.;
    float offset = (x - center) / width;
    return height * max(1. - offset * offset, 0.);
}