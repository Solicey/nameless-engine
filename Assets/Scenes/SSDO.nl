Scene: Untitled
Entities:
  - ID: 3472773392530139733
    Name: Entity
    TransformComponent:
      Translation: [-2.48454714, 1.59288776, 3.05362368]
      Rotation: [0, 0, 0]
      Scale: [1, 1.20000005, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.432343245, 0.645917058, 1]
      Bones:
        []
  - ID: 9021933770027055054
    Name: Entity
    TransformComponent:
      Translation: [-0.42314893, 2.62343597, 0.785687089]
      Rotation: [0, 0, 0.256563634]
      Scale: [0.999999702, 0.999999702, 4.30000019]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.139692202, 0.940594077, 0.567897141]
      Bones:
        []
  - ID: 10035270213595943175
    Name: Entity
    TransformComponent:
      Translation: [-2.50136423, 1.11159003, 2.08978449e-07]
      Rotation: [-1.57079601, 0, 0]
      Scale: [10, 9.99809551, 9.99809551]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/BiggerPlane.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
      Bones:
        []
  - ID: 7263971091382091159
    Name: SceneSettings
    TransformComponent:
      Translation: [-8.80115128, 5.81415701, 2.01250124]
      Rotation: [0.589386344, 0, 1.63859665]
      Scale: [-2.27368832, 1.43940532, 2.45574498]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: SSDO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 128
            - Type: 1
              Name: u_Radius
              Value: 3.0999999
            - Type: 1
              Name: u_Threshold
              Value: 0.00999999978
            - Type: 6
              Name: u_AmbientColor
              Value: [1, 1, 1]
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            []
  - ID: 17955367600477968809
    Name: Entity
    TransformComponent:
      Translation: [-3.53149962, 2.3762567, -0.799444318]
      Rotation: [0, 0, 0]
      Scale: [0.99999702, 0.999999762, 0.999997795]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 3.31999993
      Attenuation: [6.80000019, -2.0999999, 0.699999988]
  - ID: 4608802449465100154
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]