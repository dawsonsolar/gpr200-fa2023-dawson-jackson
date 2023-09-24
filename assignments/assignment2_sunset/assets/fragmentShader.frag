#version 450
out vec4 FragColor;
in vec2 UV;
uniform vec2 aspectRatio;
uniform float sunSpeed;
uniform float Time;
uniform vec3 skyColorDay;
uniform vec3 groundColorDay;
uniform vec3 skyColorNight;
uniform vec3 groundColorNight;
uniform float sunSize;
uniform vec3 sunColor;
uniform vec3 buildingColor;

float circleSDF(vec2 p, float r)
{
    return length(p)-r;
}


void main()
{   
    // Set the right side of buildings values
    float t = mod(floor(UV.x * 12.5), 2.0);
    t = step(t, UV.y + 0.4 + (sin(UV.x) * 0.05) * 3.0);
    t = step(t, UV.x - 0.5);
    t = step(t,UV.y);
    t = 1.0 - t;
    vec3 rightSideBuilding = vec3(t / 0.5);

    
   
    // Set the left side of buildings values.
    t = mod(floor(UV.x * 12.5), 2.0);
    t = step(t, UV.y + 0.5 + (cos(UV.x) * 0.03) * 1.0);
    t = step(t, 0.5 - UV.x);
    t = 1.0 - t;
    vec3 leftSideBuilding = vec3(t / 0.5);

    // Brightness
    vec3 Buidings = mix(rightSideBuilding, leftSideBuilding,0.5);
    Buidings = mix(buildingColor,Buidings,0.5);


     vec2 UV = (2.0 * gl_FragCoord.xy - aspectRatio.xy) / aspectRatio.y;

    // Circle centers around origin (goes in a circle)
    vec2 circlePos = vec2(cos(Time * sunSpeed),sin(Time * sunSpeed) - 0.5);
    float d = circleSDF(UV - circlePos, sunSize);
    
    d = 1.0 - smoothstep(-0.05,0.05,d);
    
    // Draws sun 
    vec3 sun = vec3(sunColor * d);
   

   // Gradients for the sky colors.
    vec3 daySkyColor = mix(skyColorDay,groundColorDay, UV.y);
    vec3 nightSkyColor = mix(skyColorNight,groundColorNight, UV.y);
   
    vec3 skyColor = mix(daySkyColor, nightSkyColor, -1.0 * circlePos.y);
    
    vec3 Final = mix(skyColor, vec3(1.0),sun);
    
    Final = mix(Final,Buidings,0.5);

    FragColor = vec4(Final, 1.0);
}