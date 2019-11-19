module line(a,b=[0,0,0], width=0.05){
    hull(){ translate(a) sphere(width); translate(b) sphere(width); }
}
function dot(a,b) = a*b;
function length(a) = norm(a);
function normalize(a) = a/norm(a);
function vec2(x,y)     = [x,y];
function vec3(x,y,z)   = [x,y,z];
function vec4(x,y,z,w) = [x,y,z,w];

A0=[1,1,1];
A1=[5,3,2];
A=normalize(A1-A0);
B0=[3,2,0];
B1=[4,4,5];
B=normalize(B1-B0);
r=0.5;

%line(A0, A1, r, $fn=30);
%line(B0, B1);

cn = normalize(cross(B,A));
projection_ =         dot(B0-A0, A) * A;
rejection   = B0-A0 - dot(B0-A0, A) * A - dot(B0-A0, cn) * cn;
sec_B_rejection  = dot(B,normalize(rejection));
closest_approach = B0-B*length(rejection)  / sec_B_rejection;
entrance         = B0-B*(length(rejection) - sqrt(max(r*r-pow(dot(B0-A0, cn), 2), 0)))/ sec_B_rejection;
exit             = B0-B*(length(rejection) + sqrt(max(r*r-pow(dot(B0-A0, cn), 2), 0)))/ sec_B_rejection;

color("red")   line(A0, A0+projection_);
color("green") line(B0, B0-rejection);
color("blue")  line(A0+projection_, A0+projection_+cn);
//color("yellow")line(B0, closest_approach);
color("green")line(entrance, exit);
