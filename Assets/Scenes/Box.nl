Scene: Untitled
Entities:
  - ID: 2196302291656917860
    Name: Entity
    TransformComponent:
      Translation: [-0.236014381, 2.3592391, -1.65631258]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: PBRTexLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Textures\PBR\tj0iajlbw_2K_Albedo.jpg
            - Type: 15
              Name: u_Metalness
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Textures\PBR\tj0iajlbw_2K_Normal.jpg
            - Type: 15
              Name: u_Roughness
              Value: ../Assets\Textures\PBR\tj0iajlbw_2K_Roughness.jpg
            - Type: 1
              Name: u_IsNormalsCompressed
              Value: 0
      Bones:
        []
  - ID: 6812967650305617139
    Name: SceneSettings
    TransformComponent:
      Translation: [3.57068086, 2.32509708, -2.34519768]
      Rotation: [0.222012907, 0, 16.6893196]
      Scale: [-0.283780068, 1.27838373, 0.232453912]
    SettingsComponent:
      AntiAliasingType: 1
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: false
      RenderMode: 0
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 10250129764006599626
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
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
              Value: [0.297029674, 1, 0.422311366]
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_Roughness
              Value: 1
      Bones:
        []
  - ID: 16823263282777613680
    Name: Entity
    TransformComponent:
      Translation: [0.786752939, 2.49873614, -0.857508183]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 3
      Attenuation: [1, 0.699999988, 1.79999995]