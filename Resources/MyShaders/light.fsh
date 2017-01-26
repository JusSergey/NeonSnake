#ifdef GL_ES
precision lowp float;                                                                
#endif                                                                                                                                    
                                                                                     
varying vec2 v_texCoord;                                                             
varying vec4 v_color;                                                                
                                                                                     
uniform sampler2D u_texture;                                                         
uniform vec2 resolution;
uniform vec2 u_center1;
uniform vec2 u_center2;
uniform float maskShader;
uniform vec3 filter;

uniform float blackwhite;

void main(void) {
    vec4 outColor = texture2D(u_texture, v_texCoord);

        float dist1 = (0.9 - distance(v_texCoord, u_center1));
        float dist2 = (0.9 - distance(v_texCoord, u_center2));

        if (maskShader == 1.0) {

            if (dist1 > dist2) {
                float dist_pow_3 = dist1*dist1*dist1;

                outColor.b = ((outColor.r * dist_pow_3 * 3.0) ) + (dist1 - 0.5) / 1.5;
                outColor.g =  (outColor.g * dist_pow_3)                           * dist1;
                outColor.r = ((outColor.b * dist_pow_3)       + cos(dist1) / 4.0) * dist1;

            }
            else {
                float dist_pow_3 = dist2*dist2*dist2;

                outColor.b = ((outColor.r * dist_pow_3 * 3.0)) + (dist2 - 0.5) / 1.5;
                outColor.g = ((outColor.g * dist_pow_3)) * dist2;
                outColor.r = ((outColor.b * dist_pow_3) + cos(dist2) / 4.0) * dist2;

            }

        }

        else if (maskShader == 2.0) {
            if (dist1 > dist2) {
                float dist_pow_3 = dist1*dist1*dist1;

                outColor.g = ((outColor.r * dist_pow_3 * 3.0) ) + (dist1 - 0.5) / 1.5;
                outColor.b =  (outColor.g * dist_pow_3)                          * dist1;
                outColor.r = ((outColor.b * dist_pow_3)       + cos(dist1) / 4.0) * dist1;
            }
            else {
                float dist_pow_3 = dist2*dist2*dist2;

                outColor.g = ((outColor.r * dist_pow_3 * 3.0)) + (dist2 - 0.5) / 1.5;
                outColor.b = ((outColor.g * dist_pow_3)) * dist2;
                outColor.r = ((outColor.b * dist_pow_3) + cos(dist2) / 4.0) * dist2;
            }
        }

        else {
            if (dist1 > dist2) {
                float dist_pow_3 = dist1*dist1*dist1;

                outColor.r = ((outColor.r * dist_pow_3 * 3.0) ) + (dist1 - 0.5) / 1.5;
                outColor.g =  (outColor.g * dist_pow_3)                          * dist1;
                outColor.b = ((outColor.b * dist_pow_3)       + cos(dist1) / 4.0) * dist1;
            }
            else {
                float dist_pow_3 = dist2*dist2*dist2;

                outColor.r = ((outColor.r * dist_pow_3 * 3.0)) + (dist2 - 0.5) / 1.5;
                outColor.g = ((outColor.g * dist_pow_3)) * dist2;
                outColor.b = ((outColor.b * dist_pow_3) + cos(dist2) / 4.0) * dist2;
            }
        }

   // if (blackwhite > 0.0) {

        float b = (outColor.r + outColor.g + outColor.b) / 2.0;
        outColor.r += (b-outColor.r)*blackwhite;
        outColor.g += (b-outColor.g)*blackwhite;
        outColor.b += (b-outColor.b)*blackwhite;

//        outColor.a = min;
    //}

    gl_FragColor = outColor;
}                                                                                    

