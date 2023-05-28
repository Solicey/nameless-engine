Scene: Untitled
Entities:
  - ID: 15964453503236117756
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Bonfire/Bonfire_Ground.fbx
      Materials:
        - MaterialName: o000100_00\N:\FDP\data\Model\obj\o00\o000100\tex\o000100_00_a.t
          ShaderName: TexLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 1
              Name: u_AmbientStrength
              Value: 1.29999995
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Bonfire\o000100_00_a.png
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Models\Bonfire\o000100_00_n.png
            - Type: 1
              Name: u_IsNormalsCompressed
              Value: 1.5
      Bones:
        []
  - ID: 18056566155329855148
    Name: Entity
    TransformComponent:
      Translation: [0.217743501, 0.526730001, -0.891889095]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 0.711792946, 0.405940592]
      Intensity: 1.47000003
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 12717913385150944146
    Name: SceneSettings
    TransformComponent:
      Translation: [-1.09532773, 0.626857579, 0.23701334]
      Rotation: [-0.509997845, -1.35769629, 0]
      Scale: [0, 0, 0]
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