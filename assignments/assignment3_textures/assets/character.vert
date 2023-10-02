#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uUV;
out vec2 UV;

uniform float time;
uniform float speed;
uniform vec2 aspectRatio;
uniform vec2 imgSize;

void main()
{

    // Setting boundaries
    UV = uUV;
    float X = vPos.x * (imgSize.x / aspectRatio.x);
    float Y = vPos.y * (imgSize.y / aspectRatio.y);


    // Make character go in a circle.
    float radius = 0.5;
    float timeFactor = time * speed;
    float newX = X + radius * cos(timeFactor);
    float newY = Y + radius * sin(timeFactor);

    gl_Position = vec4(newX, newY, vPos.z, 1.0);
}