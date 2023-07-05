Scene: Untitled
Entities:
  - ID: 15823758581409727869
    Name: Entity
    TransformComponent:
      Translation: [2.2827692, 3.69548059, 0.0953767598]
      Rotation: [-3.09273458, -1.02209878, -2.52337766]
      Scale: [0.9999789, 0.99999249, 0.999999881]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 1.20000005
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 3392943982150419163
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/breakfast_room/breakfast_room.obj
      Materials:
        - MaterialName: breakfast_room:Paint___White_Matt
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.646864653, 0.628517509, 0.587088346]
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
        - MaterialName: breakfast_room:Material_002
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
        - MaterialName: breakfast_room:Floor_Tiles
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 0.400000006
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/tiles.png
            - Type: 1
              Name: u_UseSpecular
              Value: 0
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
        - MaterialName: breakfast_room:Chrome
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.392739296, 0.383422256, 0.36422357]
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0, 0, 0]
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.191419125, 0.177345574, 0.159831807]
        - MaterialName: breakfast_room:Gold_Paint
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.865209281, 0.58745873]
        - MaterialName: breakfast_room:Ceramic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.891089082, 0.990295053, 1]
        - MaterialName: breakfast_room:Paint___White_Gloss
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.363036275, 0.308289617, 0.277968377]
        - MaterialName: breakfast_room:Material_005
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.442244232, 0.442244232, 0.442244232]
        - MaterialName: breakfast_room:White_Marble
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 0.600000024
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/marble.jpg
            - Type: 1
              Name: u_UseSpecular
              Value: 0
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
        - MaterialName: breakfast_room:Artwork
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
              Value: ../Assets/Models/breakfast_room/picture3.jpg
      Bones:
        []
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [-4.18261909, 3.40115023, 3.31681609]
      Rotation: [0.404999077, 0, -37.2987823]
      Scale: [-2.73607278, 1.80993307, -0.101446681]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 0
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
              Value: 24.7000008
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