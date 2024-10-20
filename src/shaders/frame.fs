#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 screenResolution;
uniform sampler2D texture;
uniform vec2 textureSize;
uniform float borderThickness;

out vec4 finalColor;

const float radius = 20.0;
float halfBorderThickness = borderThickness / 2.0;

float RectSDF(vec2 pos, vec2 size, float radius)
{
    vec2 dist = abs(pos) - size + vec2(radius);
    return min(max(dist.x, dist.y), 0.0) + length(max(dist, 0.0)) - radius;
}

void main()
{
    // Center position of the frame
    vec2 position = textureSize / 2.0 + borderThickness;

    // Screenspace position
    vec2 ssp = fragTexCoord * screenResolution;

    float dist = RectSDF(ssp - position, textureSize / 2.0 + halfBorderThickness, radius - halfBorderThickness);

    vec4 imageColor = texture2D(texture, (ssp - borderThickness) / textureSize);
    vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 0.0);

    vec4 color = mix(backgroundColor, imageColor, smoothstep(-1.0, 1.0, 1.0 - (dist + halfBorderThickness)));
    finalColor = mix(fragColor, color, smoothstep(-1.0, 1.0, abs(dist) - halfBorderThickness));
}
