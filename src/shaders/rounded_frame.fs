#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 screenResolution;

out vec4 finalColor;

const vec2 borderSize = vec2(350.0, 200.0);
const vec2 position = vec2(400.0, 225.0);

const float borderThickness = 10.0;
const float radius = 20.0;

const float halfBorderThickness = borderThickness / 2.0;

float RoundedRectSDF(vec2 pos, vec2 size, float radius)
{
    vec2 d = abs(pos) - size + vec2(radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void main()
{
    float dist = RoundedRectSDF(fragTexCoord * screenResolution - position, (borderSize / 2.0) + halfBorderThickness, radius);
    dist = abs(dist) - halfBorderThickness;

    // finalColor = vec4(dist, dist, dist, 1.0);
    finalColor = mix(fragColor, vec4(0.0), smoothstep(-1.0, 1.0, dist));
}
