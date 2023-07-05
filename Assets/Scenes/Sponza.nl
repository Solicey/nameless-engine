Scene: Untitled
Entities:
  - ID: 12454486741017926062
    Name: Entity
    TransformComponent:
      Translation: [0, 1.99952626, 0]
      Rotation: [0.81395191, 0, 0]
      Scale: [1, 0.999999881, 0.999999881]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 6791577370053867193
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
      ProjectionType: 1
      OrthoSize: 10
      OrthoFar: 100
      OrthoNear: 0.00999999978
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1280
      ViewportHeight: 720
      ClearFlagType: 0
  - ID: 12418745565055627628
    Name: SceneSettings
    TransformComponent:
      Translation: [-10.0203533, 2.5116806, -1.7132113]
      Rotation: [0.185998917, 0, -4.46100378]
      Scale: [-4.94038439, 1.52474213, -0.408584774]
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
  - ID: 5713983307806275842
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [0.00999999978, 0.00999999978, 0.00999999978]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/sponza/sponza.obj
      Materials:
        - MaterialName: Material__298
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: ""
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/background.png
        - MaterialName: leaf
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_thorn_diff.png
        - MaterialName: Material__57
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/vase_plant.png
        - MaterialName: vase_round
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/vase_round.png
        - MaterialName: bricks
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/spnza_bricks_a_diff.png
        - MaterialName: column_a
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_column_a_diff.png
        - MaterialName: arch
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_arch_diff.png
        - MaterialName: ceiling
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_ceiling_a_diff.png
        - MaterialName: floor
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_floor_a_diff.png
        - MaterialName: column_c
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_column_c_diff.png
        - MaterialName: details
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_details_diff.png
        - MaterialName: column_b
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_column_b_diff.png
        - MaterialName: Material__47
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: Default
        - MaterialName: flagpole
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_flagpole_diff.png
        - MaterialName: fabric_e
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_fabric_green_diff.png
        - MaterialName: fabric_d
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_fabric_blue_diff.png
        - MaterialName: fabric_a
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_fabric_diff.png
        - MaterialName: fabric_g
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_curtain_blue_diff.png
        - MaterialName: fabric_c
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_curtain_diff.png
        - MaterialName: fabric_f
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_curtain_green_diff.png
        - MaterialName: chain
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/chain_texture.png
        - MaterialName: vase_hanging
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/vase_hanging.png
        - MaterialName: vase
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/vase_dif.png
        - MaterialName: Material__25
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/lion.png
        - MaterialName: roof
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/sponza/textures/sponza_roof_diff.png
      Bones:
        []