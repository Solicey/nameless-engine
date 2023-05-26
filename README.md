
nameless engine 是一个基于 [Hazel](https://github.com/TheCherno/Hazel)框架的游戏引擎，目前具有最基础的3D渲染功能。

## 运行环境

1. OS：Windows（仅在Win10下测试过）
2. RAM：500MB
3. GPU：支持OpenGL 4.5
4. CPU：x64

## 项目构建

0. `git clone git@github.com:Solicey/nameless-engine.git`
1. **请先确认项目路径不含中文**。
2. 开发使用VS2022作为IDE，如果您的Visual Studio是更低的版本，请修改GenerateSolution.bat文件，将'vs2022'改成对应的版本。
3. 运行GenerateSolution.bat后，同级目录下会生成Nameless.sln解决方案。
4. 打开Nameless.sln，构建项目。构建成功后，Debug/Release子目录中会生成Editor.exe应用程序。至此构建完成。

## 文档

### UI界面

#### 最上方菜单栏

- File：创建、加载或保存场景。一般将场景保存在Assets/Scene下，后缀为.nl。
- Script：重新加载C#程序集。
- Window：当前Editor中激活的所有窗口。

#### Hierarchy

- 显示当前场景中的物体条目。
- 右键空白处点击Create Entity，或者按Shift+A，可以创建新物体。
- 右键某物体条目点击Delete Entity，或者选中物体后按Shift+D，可以删除该物体。
- 右键某物体条目点击Duplicate Entity，或者选中物体后按Ctrl+D，可以复制该物体。

#### Viewport

- Viewport窗口没有显示名称，它显示当前活跃摄像机的画面。
- Nameless Editor有两种模式：Editor Mode和Game Mode。在Editor Mode下，活跃的摄像机为Editor Camera；在Game Mode下，活跃的摄像机为挂载了Camera部件的物体，可以自由地切换活跃摄像机。
- 默认情况下处于Editor模式。此时可以随意改变Editor Camera的视角。按住鼠标右键移动鼠标可以改变相机角度，按住鼠标中键移动鼠标可以改变相机位置，滚动鼠标滚轮对应缩放。
- Editor Mode下，点击Viewport上方的三角形以进入Game Mode。但是默认处于暂停状态（此时场景中的粒子系统动画不会播放、物体挂载的C#脚本不会执行Update），需要再次点击画面任意一处以进入播放状态。
- Game Mode播放状态下，光标不会显示。若想退出，请按Esc。
- Game Mode暂停状态下，点击Viewport上方的正方形以回到Editor Mode。

#### Inspector

- 选中某个物体后，Inspector窗口会显示物体的名字和组件信息。
- 点击Add Component以添加组件。
- 点击某组件名称右侧“...”，可以删除组件。（除Transform组件）

#### Scene Settings

- 包含一些全局的场景设置。如渲染模式（前向或延迟）、后处理流水线、抗锯齿选项等。

#### Resource List

- 显示内存中活跃的资源。


### 组件

- Transform：默认挂载的组件。表示物体的位移、旋转与缩放。
- Camera：挂载该组件后，Game Mode下物体可作为活跃摄像机。
- Model Renderer：在Viewport中绘制3D模型。
- Sprite Renderer：在Viewport中绘制2D Sprite。
- Post Processing：需要Camera作为前置组件。可为Game Mode下的Viewport画面提供后处理。
- Light：包括点光源和方向光源。
- Particle System：粒子系统。
- Scripting：C#脚本绑定。


### Assets 导入

- Nameless Editor所需的所有外部资源都要放在Assets文件夹下的若干个子目录中，以便程序搜索查询。**Shader一定要放在Assets/Shaders下，模型一定要放在Assets/Models下。** 但纹理可以放在Assets下的任意子文件夹中。


### NLSL语言

- NLSL着色器语言仅仅是在GLSL的基础上添加了一些补充信息，如#use、#tag、#prop、#type
- Shader文件后缀仍为“.glsl”。
- 以下为Shader文件示例：

```glsl
// nlsl template shader file

// 请勿在含“#”的行末添加注释

// 该着色器的用途（只能跟随一个用途）
// 现有的用途:
// model: 用于Model Renderer
// post: 用于后处理
// particle1: 粒子系统第一个pass
// particle2: 粒子系统第二个pass
#use model

// #tag后可以跟随多个标签，由分号隔开，标签可重复（虽然没有意义）
// 现有的标签:
// ssao: 作用于后处理shader。在shader绑定uniform时会多绑定一个噪声纹理，详见OpenGLPostProcessing.cpp
// src: 作用于后处理shader。额外绑定一张u_SrcColorTex，即后处理还未开始前待处理的图像。u_ColorTex是上一个shader处理后的图像。
#tag none;none;

// #prop与#end之间填写自定义的uniform变量，这些变量可以在Editor中实时修改
// 格式为: 类型 uniform变量名;
// 可使用的类型有: sampler2D/color3/float/int;
// 注意严格按照格式，否则可能会出错。（parser写得很垃圾）
#prop
color3 u_Color;
sampler2D u_Custom;
sampler2D u_Diffuse;
#end

// 有一些uniform是固定会绑定的，无需在#prop中声明
// 对于用途为model的shader而言，固定会绑定的uniform有：
// Camera: 内含活跃摄像机的所有信息。注意这是一个uniform buffer
// mat4 u_Transform: 当前物体的Model Matrix
// mat3 u_NormalMatrixWS: 用于将法线转到世界空间的mat3矩阵
// mat3 u_NormalMatrixVS: 用于将法线转到观察空间的mat3矩阵
// int u_EntityId: 当前物体Id，用于物体描边

// #type后接着色器的类型
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

// 以下三个是自定义的uniform，需要在#prop和#end之间重新声明一遍。
uniform vec3 u_Color;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Custom;

uniform int u_EntityId; // 必加

void main()
{
    f_Color = vec4(texture2D(u_Diffuse, v_TexCoord).rgb, 1.0) * vec4(u_Color, 1.0);
    f_EntityId = u_EntityId; // 必加
}
```

- 更多实例详见Assets/Shaders