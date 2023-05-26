Scene: Untitled
Entities:
  - ID: 1141227927795100365
    Name: SceneSettings
    TransformComponent:
      Translation: [0.669212222, -2.70763445, -4.05147839]
      Rotation: [2.24158859, 1.29241061, 1.08449285e-07]
      Scale: [1.97885036, -0.991186559, -3.67717338]
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
  - ID: 10223125187507918363
    Name: Entity
    TransformComponent:
      Translation: [0, 1.1920929e-07, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Lit(SingleDirLight).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.970297039, 0.467535853, 0.467535853]
            - Type: 1
              Name: u_AmbientStrength
              Value: 0.300000012
            - Type: 1
              Name: u_SpecularStrength
              Value: 0.200000003
      Bones:
        []
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