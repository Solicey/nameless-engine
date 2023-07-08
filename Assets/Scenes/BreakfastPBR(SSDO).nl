Scene: Untitled
Entities:
  - ID: 10718493362269171100
    Name: Cam3
    TransformComponent:
      Translation: [-2.8599999, 4, 1.78999996]
      Rotation: [-3.14159274, 0.0343829878, -3.14159274]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0117647061, 0.0117647061, 0.0117647061]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 2
            - Type: 6
              Name: u_AmbientColor
              Value: [0.329411775, 0.337254912, 0.345098048]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 9843048288253970148
    Name: Cam2
    TransformComponent:
      Translation: [1.44000006, 1.38999999, 2.1099999]
      Rotation: [0, 1.30760062, 0]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0117647061, 0.0117647061, 0.0117647061]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 2
            - Type: 6
              Name: u_AmbientColor
              Value: [0.329411775, 0.337254912, 0.345098048]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 4410601630818347464
    Name: Cam1
    TransformComponent:
      Translation: [2.97000003, -1.11000001, 2.77999997]
      Rotation: [-0.29513517, 1.14790297, -0.0555014722]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0117647061, 0.0117647061, 0.0117647061]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 2
            - Type: 6
              Name: u_AmbientColor
              Value: [0.329411775, 0.337254912, 0.345098048]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 134663924845123423
    Name: Cam0
    TransformComponent:
      Translation: [-1.13999999, 1.89999998, -3.8599999]
      Rotation: [2.82376814, -0.0530580096, -3.14159274]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0117647061, 0.0117647061, 0.0117647061]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 2
            - Type: 6
              Name: u_AmbientColor
              Value: [0.329411775, 0.337254912, 0.345098048]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [6.78242731, 0.0382566452, 0.866296411]
      Rotation: [-0.162041634, 0, -121.036774]
      Scale: [-0.379825234, 1.21339393, 1.4798522]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: false
      RenderMode: 1
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0117647061, 0.0117647061, 0.0117647061]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 2
            - Type: 6
              Name: u_AmbientColor
              Value: [0.329411775, 0.337254912, 0.345098048]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShadingPBR(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowMaxLength
              Value: 50
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 3392943982150419163
    Name: Room
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.760784328, 0.721568644, 0.631372571]
            - Type: 1
              Name: u_Roughness
              Value: 0.300000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.300000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.36470589, 0.36470589, 0.36470589]
            - Type: 1
              Name: u_Roughness
              Value: 0.800000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.90196079, 0.90196079, 0.90196079]
            - Type: 1
              Name: u_Roughness
              Value: 0.600000024
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.490196079, 0.4627451, 0.427450985]
            - Type: 1
              Name: u_Roughness
              Value: 0.800000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Ceramic
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.79207921, 0.997941494, 1]
            - Type: 1
              Name: u_Roughness
              Value: 0.300000012
            - Type: 1
              Name: u_Metallic
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Material_005
          ShaderName: PureColor(PBR).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.788235307, 0.788235307, 0.788235307]
            - Type: 1
              Name: u_Roughness
              Value: 0.5
            - Type: 1
              Name: u_Metallic
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: false
      Bones:
        []
  - ID: 15823758581409727869
    Name: Sun
    TransformComponent:
      Translation: [-1.19187057, 11.4593058, -16.3879128]
      Rotation: [0.317542404, 0.635508299, -0.121562287]
      Scale: [0.999999642, 0.999999881, 0.999999642]
    LightComponent:
      LightType: 0
      Color: [0.924092412, 0.893141389, 0.799050212]
      Intensity: 2
      Attenuation: [1, 0.699999988, 1.79999995]