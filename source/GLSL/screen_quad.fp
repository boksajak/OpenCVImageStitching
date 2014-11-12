#version 400

uniform sampler2D screenTexture;
in vec2 v_texCoord;

out vec4 outputColor;

uniform vec2 nearFar;

// Render passes
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

vec4 mapDepthToColor() {
    // Map depth value into visible range

    vec4 uZBounds = vec4(nearFar.x, nearFar.y, nearFar.x + nearFar.y, nearFar.y - nearFar.x); // near, far, far+near, far-near

    float z = texture(screenTexture, v_texCoord).r;
    z = (2.0 * uZBounds[0]) / (uZBounds[2] - z * uZBounds[3]);

    return vec4(z, z, z, 1);
}

subroutine (RenderPassType)
void NormalPass(){
    outputColor = texture(screenTexture, v_texCoord).rgba;
}

subroutine (RenderPassType)
void DepthPass(){
    outputColor = mapDepthToColor();
}

void main(){
    RenderPass();
}
