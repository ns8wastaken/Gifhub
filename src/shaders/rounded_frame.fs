#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

uniform vec2 screenResolution;

vec2 uv = vec2(0); // centered pixel position -1 .. 1

// functions return intensity (0.0 .. 1.0) with antialiased edges

float roundedRectangle(vec2 pos, vec2 size, float radius, float thickness) {
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.66, 0.33, d / thickness * 5.0);
}

float roundedFrame(vec2 pos, vec2 size, float radius, float thickness) {
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}

//---------------------------------------------------------
void main() {
    vec2 pos, size;

    // get uv position with origin at window center
    vec2 ratio = vec2(screenResolution.x / screenResolution.y, 1.0);          // aspect ratio (x/y,1)
    uv = (2.0 * (fragTexCoord.xy / screenResolution.xy) - 1.0) * ratio;     // -1.0 .. 1.0

    vec3 col;

    float intensity = 0.0;

    //--- rounded rectangle ---
    // pos = vec2(0.4, 0.6);
    pos = vec2(0);
    size = vec2(0.5, 0.2);
    intensity = 0.6 * roundedRectangle(pos, size, 0.1, 0.2);
    col = mix(col, fragColor.xyz, intensity);

    //--- rounded frame ---
    intensity = roundedFrame(pos, size, 0.08, 0.2);
    col = mix(col, fragColor.xyz, intensity);

    finalColor = vec4(col, 1.0);
}
