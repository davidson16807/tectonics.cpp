
// This function returns a rgb vector that quickly approximates a spectral "bump".
// Adapted from GPU Gems and Alan Zucconi
// from https://www.alanzucconi.com/2017/07/15/improving-the-rainbow/
FUNC(float) bump (
    IN(float) x, 
    IN(float) edge0, 
    IN(float) edge1, 
    IN(float) height
){
    VAR(float) center = (edge1 + edge0) / 2.;
    VAR(float) width  = (edge1 - edge0) / 2.;
    VAR(float) offset = (x - center) / width;
    return height * max(1. - offset * offset, 0.);
}