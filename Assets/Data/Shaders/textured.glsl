#version 330 core

vert 
[
    layout (location = 0) in vec3 iPosition;
    layout (location = 1) in vec3 iNormal;
    layout (location = 2) in vec2 iTexcoords;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    out VertexData
    {
       vec2 texcoords;
    } oData;

    void main()
    {
        oData.texcoords = iTexcoords;
        
        gl_Position = projection * view * model * vec4(iPosition, 1.0f);
    }
];
    
frag 
[
    in VertexData
    {
        vec2 texcoords;
    } iData;

    uniform sampler2D diffuse;
    uniform vec3 ambient;
    
    layout (location = 0) out vec4 oDiffuseColor;

    void main()
    {    
        oDiffuseColor = vec4(texture(diffuse, iData.texcoords)) * vec4(ambient, 1.0f);
    }
];