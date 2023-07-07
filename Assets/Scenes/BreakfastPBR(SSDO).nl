Scene: Untitled
Entities:
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [3.23627377, 4.65146065, -4.84329367]
      Rotation: [0.476972729, 0, -103.182777]
      Scale: [-0.589146197, 0.449239403, 2.33243179]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 1
              Name: u_EnableShadow
              Value: 1
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 6
              Name: u_AmbientColor
              Value: [0.265032828, 0.267326713, 0.255857229]
        - ShaderName: SSDO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 1
            - Type: 1
              Name: u_Bias
              Value: 0.00899999961
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 1.20000005
            - Type: 6
              Name: u_AmbientColor
              Value: [0.568627477, 0.576470613, 0.53725493]
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 1
              Name: u_EnableShadow
              Value: 1
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 1
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
  - ID: 3392943982150419163
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [3.14159226, 0.609246016, 3.14159226]
      Scale: [0.999999523, 1, 0.999999523]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/breakfast_room/breakfast_room.obj
      Materials:
        - MaterialName: breakfast_room:Paint___White_Matt
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.600000024
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.877887785, 0.83875978, 0.750405669]
            - Type: 1
              Name: u_Roughness
              Value: 0.300000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.400000006
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Material_002
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.400000006
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Floor_Tiles
          ShaderName: TexPBR(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/tiles.png
            - Type: 15
              Name: u_Normals
              Value: ""
            - Type: 1
              Name: u_Roughness
              Value: 0.400000006
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
        - MaterialName: breakfast_room:Chrome
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.501650155, 0.501650155, 0.501650155]
            - Type: 1
              Name: u_Roughness
              Value: 0.400000006
            - Type: 1
              Name: u_Metallic
              Value: 1
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.471947193, 0.471947193, 0.471947193]
            - Type: 1
              Name: u_Roughness
              Value: 0.800000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.900990129, 0.900990129, 0.900990129]
            - Type: 1
              Name: u_Roughness
              Value: 0.600000024
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.501650155, 0.46329242, 0.415558398]
            - Type: 1
              Name: u_Roughness
              Value: 0.800000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Gold_Paint
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.865209281, 0.58745873]
            - Type: 1
              Name: u_Roughness
              Value: 0.400000006
            - Type: 1
              Name: u_Metallic
              Value: 1
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Ceramic
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.79207921, 0.997941494, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.5
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Paint___White_Gloss
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.800000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:Material_005
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.788778901, 0.788778901, 0.788778901]
            - Type: 1
              Name: u_Roughness
              Value: 0.5
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
        - MaterialName: breakfast_room:White_Marble
          ShaderName: TexPBR(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/marble.jpg
            - Type: 15
              Name: u_Normals
              Value: ""
            - Type: 1
              Name: u_Roughness
              Value: 0.300000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
        - MaterialName: breakfast_room:Artwork
          ShaderName: TexPBR(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/picture3.jpg
            - Type: 15
              Name: u_Normals
              Value: ""
            - Type: 1
              Name: u_Roughness
              Value: 0.600000024
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
      Bones:
        []
  - ID: 15823758581409727869
    Name: Entity
    TransformComponent:
      Translation: [-1.19187057, 11.4593058, -16.3879128]
      Rotation: [0.317542404, 0.635508299, -0.121562287]
      Scale: [0.999999642, 0.999999881, 0.999999642]
    LightComponent:
      LightType: 0
      Color: [0.924092412, 0.893141389, 0.799050212]
      Intensity: 2
      Attenuation: [1, 0.699999988, 1.79999995]