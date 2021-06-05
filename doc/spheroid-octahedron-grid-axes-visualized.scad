
module line(start, end=[0,0,0], thickness = 0.01) {
    hull() {
        translate(start) sphere(thickness);
        translate(end) sphere(thickness);
    }
}



%scale(1000)
polyhedron(
    points = [
        [ 1, 0, 0],
        [-1, 0, 0],
        [ 0, 1, 0],
        [ 0,-1, 0],
        [ 0, 0, 1],
        [ 0, 0,-1],
    ],
    faces = [
        [0, 2, 4],
        [0, 2, 5],
        [0, 3, 4],
        [0, 3, 5],
        [1, 2, 4],
        [1, 2, 5],
        [1, 3, 4],
        [1, 3, 5],
    ]
);

top=[0,0,1]*1e3;
bottom=[0,0,-1]*1e3;
corner_px=[1,0,0]*1e3;
corner_nx=[-1,0,0]*1e3;
corner_py=[0,1,0]*1e3;
corner_ny=[0,-1,0]*1e3;

// line(corner_px, top, 30);
// line(corner_py, top, 30);

line(bottom, corner_py, 30);
line(bottom, corner_px, 30);