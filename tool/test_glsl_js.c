const float PI = 3.141592653589793238462643383279;

float test_math_library_functions() 
{
    float a = 1.0f;
    float b = 1.0f;
    float c = PI*a;
    float d = exp(-a/b);
    float e = sin(c);
    float max = max(a*b,c);
    return max(sin(c), exp(-a/b));
}
float test_glm_library_functions() 
{
    float a = 1.0f;
    float b = 1.0f;
    float c = PI*a;
    float d = degrees(c);
    vec3  A = vec3(1,2,3);
    vec3  B = vec3(1.0f);
    vec3  C = cross(d, -B);
    float e = dot(d*A, A*B);
    return dot(d*A, A*B);
}
float test_attribute_expression() 
{
    vec3  a = vec3(1,2,3);
    float b = a.x;
    float c = a[0];
    mat3  A = mat3(1);
    vec3  B = A.x;
    vec3  B = A[0];
    float d = A.x.y;
    float e = A[0].y;
    return mat3(A,B,vec3(0)).x[0];
}
float test_unary_operators() 
{
    float a = 1.0f;
    float b = 1.0f;
    float c = 1.0f;
    int i = 0.0;
    // i++;
    // i--;
    // c+=a;
    // c-=-a;
    // c*=+b;

    vec3 A = vec3(1.0f);
    vec3 B = vec3(1.0f);
    // A++;
    // B--;
    // A+=B;
    // A-=-B;
    // A*=+B;

    mat3 D = mat3(1.0f);
    mat3 E = mat3(1.0f);
    // D++;
    // E--;
    // D+=E;
    // D-=-E;
    // D*=+E;
    
    return 1.0f;
}

float test_binary_operators() 
{
    float a = 1.0f;
    float b = 1.0f;
    float c = 1.0f;
    int i = 0.0;
    c = a+b;
    c = a-b;
    c = a*b;
    c = a/b;
    
    vec3 A = vec3(1.0f);
    vec3 B = vec3(1.0f);
    vec3 C = vec3(1.0f);
    C = A+B;
    C = A-B;
    C = A*B;
    C = A/B;
    C = vec3(1.0f)+B;
    C = vec3(1.0f)-B;
    C = vec3(1.0f)*B;
    C = vec3(1.0f)/B;

    mat3 D = mat3(1.0f);
    mat3 E = mat3(1.0f);
    mat3 F = mat3(1.0f);
    F = D+E;
    F = D-E;
    F = D*E;
    F = D/E;
    F = mat3(1.0f)+E;
    F = mat3(1.0f)-E;
    F = mat3(1.0f)*E;
    F = mat3(1.0f)/E;
    C = D.x+B;
    C = D.x-B;
    C = D.x*B;
    C = D.x/B;
    C = D[0]+B;
    C = D[0]-B;
    C = D[0]*B;
    C = D[0]/B;
    C = mat3(1.0f).x+B;
    C = mat3(1.0f).x-B;
    C = mat3(1.0f).x*B;
    C = mat3(1.0f).x/B;
    C = mat3(1.0f)[0]+B;
    C = mat3(1.0f)[0]-B;
    C = mat3(1.0f)[0]*B;
    C = mat3(1.0f)[0]/B;
    
    return 1.0f;
}

float test_variable_declaration()
{
    const float A = 1.0f;
    float b = 1.0f;
}

struct TestStructureDeclaration
{
    bool a;
    float b;
    vec3 A;
    mat3 B;
};

// test function declaration
float test_function_declaration(
    bool a,
    in float b,
    in float[2] c,
    out float d,
    in vec3 A,
    in vec3[2] B,
    out mat3 C
){
    return 1.0f;
}

