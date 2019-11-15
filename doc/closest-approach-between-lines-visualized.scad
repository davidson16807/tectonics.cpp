module line(a,b=[0,0,0], width=0.05){
    hull(){ translate(a) sphere(width); translate(b) sphere(width); }
}
module point(a, width=0.05){
    translate(a) sphere(width);
}
function dot(a,b) = a*b;
function normalize(a) = a/norm(a);

a0=[1,1,1];
a1=[5,3,2];
an=normalize(a1-a0);
b0=[3,2,0];
b1=[4,4,5];
bn=normalize(b1-b0);
%line(a0, a1);
%line(b0, b1);

cn = normalize(cross(bn,an));
projection_ =         dot(b0-a0, an) * an;
rejection   = b0-a0 - dot(b0-a0, an) * an - dot(b0-a0, cn) * cn;
line(a0, a0+projection_);
line(b0, b0-rejection);
line(a0+projection_, a0+projection_+cn);
line(b0, b0-bn*norm(rejection)/dot(bn,normalize(rejection)));

