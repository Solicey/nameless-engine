Scene: Untitled
Entities:
  - ID: 6348400089869497228
    Name: SceneSettings
    TransformComponent:
      Translation: [3.29049635, 19.6746464, 34.9845581]
      Rotation: [0.467993796, 0, -50.0847969]
      Scale: [10.1036673, 0.511577129, -2.31153059]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        - ShaderName: SSDO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 10
            - Type: 1
              Name: u_Bias
              Value: 0.00999999978
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_DirectLightWeight
              Value: 1
            - Type: 1
              Name: u_IndirectLightWeight
              Value: 5
            - Type: 6
              Name: u_AmbientColor
              Value: [0.640329361, 0.678502679, 0.683168292]
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
              Value: 3
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 1
  - ID: 6213896353434949577
    Name: Entity
    TransformComponent:
      Translation: [22.0733871, -2.66050108e-07, 9.16891289]
      Rotation: [0, 0, 0]
      Scale: [39.2999992, 0.999999881, 28.6000004]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Plane.obj
      Materials:
        - MaterialName: None
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.598802805, 0.679867983, 0.587872684]
            - Type: 1
              Name: u_Specular
              Value: 0.300000012
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
      Bones:
        []
  - ID: 13847465691074950322
    Name: Entity
    TransformComponent:
      Translation: [-17.1233959, 37.1884956, -13.9176025]
      Rotation: [0.702328742, 0.260870248, 0.685190141]
      Scale: [0.999999881, 0.999999881, 0.999999881]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 12802549881112971347
    Name: Entity
    TransformComponent:
      Translation: [8.15429497, -5.19794273, 7.37737131]
      Rotation: [-0.0342065692, 1.34335375, 1.53568149]
      Scale: [1.00000012, 1.00002396, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Hand.fbx
      Materials:
        - MaterialName: 02 - Default
          ShaderName: PureColor.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.455445528, 0.455445528]
            - Type: 1
              Name: u_Specular
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 1
            - Type: 1
              Name: u_EnableGammaCorrection
              Value: 0
      Bones:
        - Parent: "null"
          Child: metacarpals
        - Parent: metacarpals
          Child: thumb_proximal_phalange
        - Parent: thumb_intermediate_phalange
          Child: thumb_distal_phalange
        - Parent: thumb_proximal_phalange
          Child: thumb_intermediate_phalange
        - Parent: metacarpals
          Child: ring_proximal_phalange
        - Parent: metacarpals
          Child: middle_proximal_phalange
        - Parent: metacarpals
          Child: index_proximal_phalange
        - Parent: metacarpals
          Child: pinky_proximal_phalange
        - Parent: index_intermediate_phalange
          Child: index_distal_phalange
        - Parent: index_proximal_phalange
          Child: index_intermediate_phalange
        - Parent: middle_intermediate_phalange
          Child: middle_distal_phalange
        - Parent: middle_proximal_phalange
          Child: middle_intermediate_phalange
        - Parent: ring_intermediate_phalange
          Child: ring_distal_phalange
        - Parent: ring_proximal_phalange
          Child: ring_intermediate_phalange
        - Parent: pinky_intermediate_phalange
          Child: pinky_distal_phalange
        - Parent: pinky_proximal_phalange
          Child: pinky_intermediate_phalange