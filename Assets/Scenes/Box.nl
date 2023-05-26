Scene: Untitled
Entities:
  - ID: 9613750654330948600
    Name: Entity
    TransformComponent:
      Translation: [2.06991029, 2.02635312, 1.67356014]
      Rotation: [0.864566684, 7.74860382e-07, -0.946796238]
      Scale: [0.999999881, 0.999999821, 0.999999881]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 10223125187507918363
    Name: Entity
    TransformComponent:
      Translation: [2.32830644e-10, 1.18277967e-07, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Template.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.831683159, 0.831683159]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Textures\MilkyWay.jpg
      Bones:
        []
  - ID: 1141227927795100365
    Name: SceneSettings
    TransformComponent:
      Translation: [-3.3153429, -1.83417928, 0.961256266]
      Rotation: [-0.16498889, 1.23717201, 3.14159274]
      Scale: [3.02045727, -0.717691779, -1.23460269]
    SettingsComponent:
      AntiAliasingType: 1
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 0
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []