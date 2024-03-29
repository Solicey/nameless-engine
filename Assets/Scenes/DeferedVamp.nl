Scene: Untitled
Entities:
  - ID: 10440442477373005482
    Name: Entity
    TransformComponent:
      Translation: [0, 12.2700138, 14.8968887]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: SSAO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 1
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSAO).glsl
          ShaderProperties:
            []
  - ID: 10740630454451859384
    Name: Light
    TransformComponent:
      Translation: [-0.852597773, 16.1530685, 2.1991806]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 6719405073046164596
    Name: Vampire
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [-3.14159203, 0.456436574, 3.14159226]
      Scale: [0.100000001, 0.100000001, 0.100000001]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/vampire/Vampire.fbx
      Materials:
        - MaterialName: Vampire_MAT1
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/vampire/test\textures\Vampire_diffuse.png
            - Type: 15
              Name: u_Specular
              Value: ../Assets/Models/vampire/test\textures\Vampire_specular.png
            - Type: 15
              Name: u_Normals
              Value: ../Assets/Models/vampire/test\textures\Vampire_normal.png
        - MaterialName: Vampire_MAT_Transparent
          ShaderName: Tex(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/vampire/test\textures\Vampire_diffuse_transparent.png
            - Type: 15
              Name: u_Specular
              Value: ../Assets/Models/vampire/test\textures\Vampire_specular.png
            - Type: 15
              Name: u_Normals
              Value: ../Assets/Models/vampire/test\textures\Vampire_normal.png
      Bones:
        - Parent: "null"
          Child: mixamorig:Hips
        - Parent: mixamorig:Hips
          Child: mixamorig:Spine
        - Parent: mixamorig:Spine
          Child: mixamorig:Spine1
        - Parent: mixamorig:Spine1
          Child: mixamorig:Spine2
        - Parent: mixamorig:Spine2
          Child: mixamorig:Neck
        - Parent: mixamorig:Neck
          Child: mixamorig:Head
        - Parent: mixamorig:Head
          Child: mixamorig:HeadTop_End
        - Parent: mixamorig:Head
          Child: LeftEye
        - Parent: mixamorig:Head
          Child: RightEye
        - Parent: mixamorig:Spine2
          Child: mixamorig:LeftShoulder
        - Parent: mixamorig:LeftShoulder
          Child: mixamorig:LeftArm
        - Parent: mixamorig:LeftArm
          Child: mixamorig:LeftForeArm
        - Parent: mixamorig:LeftForeArm
          Child: mixamorig:LeftHand
        - Parent: mixamorig:LeftHand
          Child: mixamorig:LeftHandThumb1
        - Parent: mixamorig:LeftHandThumb1
          Child: mixamorig:LeftHandThumb2
        - Parent: mixamorig:LeftHandThumb2
          Child: mixamorig:LeftHandThumb3
        - Parent: mixamorig:LeftHandThumb3
          Child: mixamorig:LeftHandThumb4
        - Parent: mixamorig:LeftHand
          Child: mixamorig:LeftHandIndex1
        - Parent: mixamorig:LeftHandIndex1
          Child: mixamorig:LeftHandIndex2
        - Parent: mixamorig:LeftHandIndex2
          Child: mixamorig:LeftHandIndex3
        - Parent: mixamorig:LeftHandIndex3
          Child: mixamorig:LeftHandIndex4
        - Parent: mixamorig:LeftHand
          Child: mixamorig:LeftHandMiddle1
        - Parent: mixamorig:LeftHandMiddle1
          Child: mixamorig:LeftHandMiddle2
        - Parent: mixamorig:LeftHandMiddle2
          Child: mixamorig:LeftHandMiddle3
        - Parent: mixamorig:LeftHandMiddle3
          Child: mixamorig:LeftHandMiddle4
        - Parent: mixamorig:LeftHand
          Child: mixamorig:LeftHandRing1
        - Parent: mixamorig:LeftHandRing1
          Child: mixamorig:LeftHandRing2
        - Parent: mixamorig:LeftHandRing2
          Child: mixamorig:LeftHandRing3
        - Parent: mixamorig:LeftHandRing3
          Child: mixamorig:LeftHandRing4
        - Parent: mixamorig:LeftHand
          Child: mixamorig:LeftHandPinky1
        - Parent: mixamorig:LeftHandPinky1
          Child: mixamorig:LeftHandPinky2
        - Parent: mixamorig:LeftHandPinky2
          Child: mixamorig:LeftHandPinky3
        - Parent: mixamorig:LeftHandPinky3
          Child: mixamorig:LeftHandPinky4
        - Parent: mixamorig:Spine2
          Child: mixamorig:RightShoulder
        - Parent: mixamorig:RightShoulder
          Child: mixamorig:RightArm
        - Parent: mixamorig:RightArm
          Child: mixamorig:RightForeArm
        - Parent: mixamorig:RightForeArm
          Child: mixamorig:RightHand
        - Parent: mixamorig:RightHand
          Child: mixamorig:RightHandThumb1
        - Parent: mixamorig:RightHandThumb1
          Child: mixamorig:RightHandThumb2
        - Parent: mixamorig:RightHandThumb2
          Child: mixamorig:RightHandThumb3
        - Parent: mixamorig:RightHandThumb3
          Child: mixamorig:RightHandThumb4
        - Parent: mixamorig:RightHand
          Child: mixamorig:RightHandIndex1
        - Parent: mixamorig:RightHandIndex1
          Child: mixamorig:RightHandIndex2
        - Parent: mixamorig:RightHandIndex2
          Child: mixamorig:RightHandIndex3
        - Parent: mixamorig:RightHandIndex3
          Child: mixamorig:RightHandIndex4
        - Parent: mixamorig:RightHand
          Child: mixamorig:RightHandMiddle1
        - Parent: mixamorig:RightHandMiddle1
          Child: mixamorig:RightHandMiddle2
        - Parent: mixamorig:RightHandMiddle2
          Child: mixamorig:RightHandMiddle3
        - Parent: mixamorig:RightHandMiddle3
          Child: mixamorig:RightHandMiddle4
        - Parent: mixamorig:RightHand
          Child: mixamorig:RightHandRing1
        - Parent: mixamorig:RightHandRing1
          Child: mixamorig:RightHandRing2
        - Parent: mixamorig:RightHandRing2
          Child: mixamorig:RightHandRing3
        - Parent: mixamorig:RightHandRing3
          Child: mixamorig:RightHandRing4
        - Parent: mixamorig:RightHand
          Child: mixamorig:RightHandPinky1
        - Parent: mixamorig:RightHandPinky1
          Child: mixamorig:RightHandPinky2
        - Parent: mixamorig:RightHandPinky2
          Child: mixamorig:RightHandPinky3
        - Parent: mixamorig:RightHandPinky3
          Child: mixamorig:RightHandPinky4
        - Parent: mixamorig:Spine1
          Child: mixamorig:L_horn1
        - Parent: mixamorig:L_horn1
          Child: mixamorig:L_horn2
        - Parent: mixamorig:Spine1
          Child: mixamorig:R_horn1
        - Parent: mixamorig:R_horn1
          Child: mixamorig:R_horn2
        - Parent: mixamorig:Hips
          Child: mixamorig:LeftUpLeg
        - Parent: mixamorig:LeftUpLeg
          Child: mixamorig:LeftLeg
        - Parent: mixamorig:LeftLeg
          Child: mixamorig:LeftFoot
        - Parent: mixamorig:LeftFoot
          Child: mixamorig:LeftToeBase
        - Parent: mixamorig:LeftToeBase
          Child: mixamorig:LeftToe_End
        - Parent: mixamorig:Hips
          Child: mixamorig:RightUpLeg
        - Parent: mixamorig:RightUpLeg
          Child: mixamorig:RightLeg
        - Parent: mixamorig:RightLeg
          Child: mixamorig:RightFoot
        - Parent: mixamorig:RightFoot
          Child: mixamorig:RightToeBase
        - Parent: mixamorig:RightToeBase
          Child: mixamorig:RightToe_End
        - Parent: mixamorig:Hips
          Child: mixamorig:L_C_skirt1
        - Parent: mixamorig:L_C_skirt1
          Child: mixamorig:L_C_skirt2
        - Parent: mixamorig:L_C_skirt2
          Child: mixamorig:L_C_skirt3
        - Parent: mixamorig:L_C_skirt3
          Child: mixamorig:L_C_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:L_A_skirt1
        - Parent: mixamorig:L_A_skirt1
          Child: mixamorig:L_A_skirt2
        - Parent: mixamorig:L_A_skirt2
          Child: mixamorig:L_A_skirt3
        - Parent: mixamorig:L_A_skirt3
          Child: mixamorig:L_A_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:L_B_skirt1
        - Parent: mixamorig:L_B_skirt1
          Child: mixamorig:L_B_skirt2
        - Parent: mixamorig:L_B_skirt2
          Child: mixamorig:L_B_skirt3
        - Parent: mixamorig:L_B_skirt3
          Child: mixamorig:L_B_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:R_C_skirt1
        - Parent: mixamorig:R_C_skirt1
          Child: mixamorig:R_C_skirt2
        - Parent: mixamorig:R_C_skirt2
          Child: mixamorig:R_C_skirt3
        - Parent: mixamorig:R_C_skirt3
          Child: mixamorig:R_C_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:R_A_skirt1
        - Parent: mixamorig:R_A_skirt1
          Child: mixamorig:R_A_skirt2
        - Parent: mixamorig:R_A_skirt2
          Child: mixamorig:R_A_skirt3
        - Parent: mixamorig:R_A_skirt3
          Child: mixamorig:R_A_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:R_B_skirt1
        - Parent: mixamorig:R_B_skirt1
          Child: mixamorig:R_B_skirt2
        - Parent: mixamorig:R_B_skirt2
          Child: mixamorig:R_B_skirt3
        - Parent: mixamorig:R_B_skirt3
          Child: mixamorig:R_B_skirt4
        - Parent: mixamorig:Hips
          Child: mixamorig:F_skirt1
        - Parent: mixamorig:F_skirt1
          Child: mixamorig:F_skirt2
        - Parent: mixamorig:F_skirt2
          Child: mixamorig:F_skirt3
        - Parent: mixamorig:F_skirt3
          Child: mixamorig:F_skirt4
  - ID: 18128042128108063409
    Name: SceneSettings
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: SSAO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 20.1000004
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSAO).glsl
          ShaderProperties:
            []