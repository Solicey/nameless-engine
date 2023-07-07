Scene: Untitled
Entities:
  - ID: 15823758581409727869
    Name: Entity
    TransformComponent:
      Translation: [3.23323917, 6.34224081, -9.39879417]
      Rotation: [0.24521482, -0.320318311, 0.21693854]
      Scale: [1.00000584, 1.00000525, 1.0000118]
    LightComponent:
      LightType: 0
      Color: [0.924092412, 0.893141389, 0.799050212]
      Intensity: 1.60000002
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 3392943982150419163
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 1.57079637, 0]
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
            - Type: 1
              Name: u_Specular
              Value: 0.100000001
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.762376249, 0.722218573, 0.631539404]
            - Type: 1
              Name: u_Specular
              Value: 0.600000024
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 3
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Material_002
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 4
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
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
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
        - MaterialName: breakfast_room:Chrome
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.75907588, 0.75907588, 0.75907588]
            - Type: 1
              Name: u_Specular
              Value: 3
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.333333313, 0.333333313, 0.333333313]
            - Type: 1
              Name: u_Specular
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.900990129, 0.900990129, 0.900990129]
            - Type: 1
              Name: u_Specular
              Value: 0.5
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.488448858, 0.461874098, 0.428803295]
            - Type: 1
              Name: u_Specular
              Value: 0.200000003
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Gold_Paint
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.865209281, 0.58745873]
            - Type: 1
              Name: u_Specular
              Value: 4
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Ceramic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.858085811, 0.99859494, 1]
            - Type: 1
              Name: u_Specular
              Value: 1.29999995
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Paint___White_Gloss
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Material_005
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.785478532, 0.785478532, 0.785478532]
            - Type: 1
              Name: u_Specular
              Value: 4
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:White_Marble
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 2
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
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
        - MaterialName: breakfast_room:Artwork
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 0.300000012
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/picture3.jpg
            - Type: 1
              Name: u_UseSpecular
              Value: 0
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
      Bones:
        []
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [1.01495671, 4.72652388, 0.0372378826]
      Rotation: [0.758998156, 0, -64.7399445]
      Scale: [-3.06588626, 0.624520838, 1.46837437]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
      ShadowZMult: 5
    PostProcessingComponent:
      Materials:
        - ShaderName: SSDO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 0.800000012
            - Type: 1
              Name: u_Threshold
              Value: 0.0199999996
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 1
              Name: u_EnableShadow
              Value: 1
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 4
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 1