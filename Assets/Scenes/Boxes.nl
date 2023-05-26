Scene: Untitled
Entities:
  - ID: 18124393584623333441
    Name: SceneSettings
    TransformComponent:
      Translation: [-3.39929104, -2.74381351, -5.26088142]
      Rotation: [2.47140503, 0.655220032, 1.87929121e-08]
      Scale: [-1.55473137, -0.344597071, -2.86059237]
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
  - ID: 13425204165480278062
    Name: Entity
    TransformComponent:
      Translation: [0.00207474828, 0.011537075, -0.00152700394]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 18288683489370970348
    Name: Entity
    TransformComponent:
      Translation: [-1.06715369, -3.7252903e-09, -3.7252903e-09]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 0
      Color: [1, 0.158415854, 0.158415854]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 666324961821051428
    Name: Entity
    TransformComponent:
      Translation: [2.21337366, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Lit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.665130854, 1, 0.445544541]
            - Type: 1
              Name: u_AmbientStrength
              Value: 1.20000005
            - Type: 1
              Name: u_SpecularStrength
              Value: 3.0999999
      Bones:
        []
  - ID: 2975794897273768246
    Name: Entity
    TransformComponent:
      Translation: [-2.96484876, -0.471862584, -1.43182385]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Lit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.445544541, 0.840799928, 1]
            - Type: 1
              Name: u_AmbientStrength
              Value: 1.20000005
            - Type: 1
              Name: u_SpecularStrength
              Value: 3.0999999
      Bones:
        []