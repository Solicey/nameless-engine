Scene: Untitled
Entities:
  - ID: 10084205020152807277
    Name: Entity
    TransformComponent:
      Translation: [1.51004064, 0.241707101, 1.56797791]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [0.464442521, 0.612545907, 0.858085811]
      Intensity: 2.63000011
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 7897188684323900489
    Name: Entity
    TransformComponent:
      Translation: [0, -2.42987347, 0]
      Rotation: [0, 0, 0]
      Scale: [16.6000004, 1, 13.5]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: PBRLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Albedo
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_Roughness
              Value: 0.5
      Bones:
        []
  - ID: 12310758108888348603
    Name: Entity
    TransformComponent:
      Translation: [0, -0.443118453, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: PBRLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Albedo
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_Roughness
              Value: 1
      Bones:
        []
  - ID: 2616126328119641547
    Name: SceneSettings
    TransformComponent:
      Translation: [4.53509951, 4.35855055, 3.87982678]
      Rotation: [0.629999816, 0, -38.514019]
      Scale: [-0.00687630568, -0.192492098, -0.401700348]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: false
      RenderMode: 0
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        []