module line(a,b=[0,0,0], width=0.05){
    hull(){ translate(a) sphere(width); translate(b) sphere(width); }
}
function dot(a,b) = a*b;
function length(a) = norm(a);
function normalize(a) = a/norm(a);
function vec2(x,y)     = [x,y];
function vec3(x,y,z)   = [x,y,z];
function vec4(x,y,z,w) = [x,y,z,w];

A0=[3,2,0];
A1=[4,4,5];
A=normalize(A1-A0);
B0=[1,1,1];
B1=[5,3,2];
B=normalize(B1-B0);
r=0.5;

%line(A0, A1);
%line(B0, B1, r, $fn=30);

// simplify the problem by using a coordinate system based around the lines

I = B;
J = normalize(cross(A,B));
K = normalize(cross(J,B));
A0I = dot(A0-B0, I);
A0J = dot(A0-B0, J);
A0K = dot(A0-B0, K);
AK  = dot(A,K);
distance_to_exit = sqrt(max(r*r-A0J*A0J, 0));
r_in    = ( distance_to_exit - A0K) / AK;
r_out   = (-distance_to_exit - A0K) / AK;

color("red")   line(B0, B0 + A0I*I);
color("green") line(A0, A0 - A0K*K);
color("blue")  line(B0 + A0I*I, B0 + A0I*I+J);
//color("yellow")line(A0, A0+A0K*A);
color("yellow")line(A0+A*r_in, A0+A*r_out);
