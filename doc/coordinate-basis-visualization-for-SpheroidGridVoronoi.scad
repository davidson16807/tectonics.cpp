function normalize(v) = v/norm(v);
x=[1,0,0];
y=[0,1,0];
z=[0,0,1];
zs=normalize([1,1,1]);
xs= normalize(cross(zs,z));
ys=-normalize(cross(zs,xs));

module line(start, end=[0,0,0], thickness = 0.01) {
    hull() {
        translate(start) sphere(thickness);
        translate(end) sphere(thickness);
    }
}

%intersection(){
    cube(1);
    sphere(1,$fn=50);
}
line(xs);
line(ys);
line(zs);
line(-xs);
line(-ys);
echo(x*zs);
echo(1/sqrt(3));$