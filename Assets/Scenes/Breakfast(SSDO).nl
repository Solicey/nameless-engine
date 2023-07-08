Scene: Untitled
Entities:
  - ID: 2840603260441263416
    Name: Cam3
    TransformComponent:
      Translation: [-2.85719204, 3.99783349, 1.79295683]
      Rotation: [-3.14159226, 0.0344568677, -3.14159226]
      Scale: [0.999938965, 1, 0.999943793]
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
        - ShaderName: PreDeferredShading(SSDO).glsl
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
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 8215802297156295386
    Name: Cam1
    TransformComponent:
      Translation: [2.97141361, -1.11326456, 2.77661085]
      Rotation: [-0.295211285, 1.14791894, -0.0554163158]
      Scale: [0.999881268, 1, 0.999896109]
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
        - ShaderName: PreDeferredShading(SSDO).glsl
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
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 967738412592709033
    Name: Cam2
    TransformComponent:
      Translation: [1.44000006, 1.38999999, 2.1099999]
      Rotation: [0, 1.30760062, 0]
      Scale: [0.999992371, 1, 1.00000262]
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
        - ShaderName: PreDeferredShading(SSDO).glsl
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
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 14029464816764848197
    Name: Cam0
    TransformComponent:
      Translation: [-1.14412618, 1.8976115, -3.85600901]
      Rotation: [2.82369566, -0.0530759469, -3.14159226]
      Scale: [0.999914408, 0.999999881, 0.999896348]
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
        - ShaderName: PreDeferredShading(SSDO).glsl
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
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: true
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [6.61893368, 0.082308948, 1.42084444]
      Rotation: [-0.123026945, 0, -102.084816]
      Scale: [-1.08031428, 1.03446853, 1.2903657]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: false
      RenderMode: 1
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        - ShaderName: PreDeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
            - Type: 6
              Name: u_AmbientColor
              Value: [0.0132012963, 0.0132012963, 0.0132012963]
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
              Value: [0.330773652, 0.337929279, 0.343234301]
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            - Type: 14
              Name: u_EnableShadow
              Value: true
            - Type: 1
              Name: u_ShadowBiasModifier
              Value: 10
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
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 0.100000001
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.762376249, 0.722218573, 0.631539404]
            - Type: 1
              Name: u_Specular
              Value: 0.600000024
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Material_002
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
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
            - Type: 14
              Name: u_UseSpecular
              Value: false
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
        - MaterialName: breakfast_room:Chrome
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.501650155, 0.501650155, 0.501650155]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.36470589, 0.36470589, 0.36470589]
            - Type: 1
              Name: u_Specular
              Value: 0
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.90196079, 0.90196079, 0.90196079]
            - Type: 1
              Name: u_Specular
              Value: 0.5
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.490196079, 0.4627451, 0.427450985]
            - Type: 1
              Name: u_Specular
              Value: 0.200000003
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Gold_Paint
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.865209281, 0.58745873]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Ceramic
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.79207921, 0.997941494, 1]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Paint___White_Gloss
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_Specular
              Value: 0.100000001
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:Material_005
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.788778901, 0.788778901, 0.788778901]
            - Type: 1
              Name: u_Specular
              Value: 1
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
            - Type: 14
              Name: u_EnableGammaCorrection
              Value: false
        - MaterialName: breakfast_room:White_Marble
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 1
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/breakfast_room/marble.jpg
            - Type: 14
              Name: u_UseSpecular
              Value: false
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
            - Type: 14
              Name: u_EnableSRGBCorrection
              Value: true
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
            - Type: 14
              Name: u_UseSpecular
              Value: false
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: Default
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
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]