// nlsl template shader file

// 请勿在含“#”的行末添加注释

// 该着色器的用途（只能跟随一个用途）
// 现有的用途:
// model: 用于Model Renderer
// post: 用于后处理
// particle1: 粒子系统第一个pass
// particle2: 粒子系统第二个pass
#use model

// tag后可以跟随多个标签，由分号隔开，标签可重复（虽然没有意义）
// 现有的标签:
// ssao: 作用于后处理shader。在shader绑定uniform时会多绑定一个噪声纹理，详见OpenGLPostProcessing.cpp
// src: 作用于后处理shader。额外绑定一张u_SrcColorTex，即后处理还未开始前待处理的图像。u_ColorTex是上一个shader处理后的图像。
#tag none;none;

// prop与end之间填写自定义的uniform变量，这些变量可以在Editor中实时修改
// 格式为: 类型 uniform变量名;
// 可使用的类型有: sampler2D/color3/float/int;
// 注意严格按照格式，否则可能会出错。（parser写得很垃圾）
#prop
color3 u_Color;
sampler2D u_Custom;
sampler2D u_Diffuse;
#end

// 有一些uniform是固定会绑定的，无需在prop中声明
// 对于用途为model的shader而言，固定会绑定的uniform有：
// Camera: 内含活跃摄像机的所有信息。注意这是一个uniform buffer
// mat4 u_Transform: 当前物体的Model Matrix
// mat3 u_NormalMatrixWS: 用于将法线转到世界空间的mat3矩阵
// mat3 u_NormalMatrixVS: 用于将法线转到观察空间的mat3矩阵
// int u_EntityId: 当前物体Id，用于物体描边

// type后接着色器的类型
// 有vertex、geometry、fragment三种。

// 顶点着色器
#type vertex

#version 450 core

// layout布局固定
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_View;
    mat4 u_Projection;
    vec3 u_CameraPosition;
    float u_Near;
    float u_Far;
};

uniform mat4 u_Transform;
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;

void main()
{
    v_Position = a_Position;
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

// 片元着色器
#type fragment

#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;

layout (location = 0) out vec4 f_Color; // 必加
layout (location = 1) out int f_EntityId; // 必加，输出物体Id用于描边

// 以下三个是自定义的uniform，需要在prop和end之间重新声明一遍。
uniform vec3 u_Color;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Custom;

uniform int u_EntityId; // 必加

void main()
{
    f_Color = vec4(texture2D(u_Diffuse, v_TexCoord).rgb, 1.0) * vec4(u_Color, 1.0);
    f_EntityId = u_EntityId; // 必加
}