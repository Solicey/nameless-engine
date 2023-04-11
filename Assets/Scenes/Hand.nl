Scene: Untitled
Entities:
  - ID: 16990954705968512128
    Name: Box
    TransformComponent:
      Translation: [9.32452965, 0, -0.515009642]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 13702378364795044523
    Name: TrackCamera
    TransformComponent:
      Translation: [-24.1449738, 2.97122717, -7.6469245]
      Rotation: [4.67775322e-07, -1.38600397, 1.17922818e-08]
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
  - ID: 7963179372585506637
    Name: FPSCamera
    TransformComponent:
      Translation: [-1.10769451, 2.16721606, 13.5970793]
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
  - ID: 1124694664969099408
    Name: Hand
    TransformComponent:
      Translation: [2.33154392, 4.33378601, -22.1060772]
      Rotation: [0.918735981, 2.98023224e-08, 1.5708009]
      Scale: [0.5, 0.5, 0.5]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Hand.fbx
      Materials:
        - MaterialName: 02 - Default
          ShaderName: DefaultWithBones.glsl
          ShaderProperties:
            []
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
    ScriptComponent:
      ClassName: HW1.Hand
      ScriptFields:
        - Name: anim_rot_speed
          Type: Float
          Data: 0
        - Name: metacarpals_rot
          Type: Vector3
          Data: [0, 0, 0]
        - Name: index_proximal_rot
          Type: Vector3
          Data: [0, 0, 0]
        - Name: thumb_distal_rot
          Type: Vector3
          Data: [0, 0, 10]
  - ID: 7976935081711616568
    Name: FixedCamera
    TransformComponent:
      Translation: [-22.1882839, 8.39671993, 16.9681034]
      Rotation: [-0.136586666, -0.63990581, -3.7152958e-07]
      Scale: [0.999999821, 1.00000012, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
      ProjectionType: 1
      OrthoSize: 10
      OrthoFar: 1
      OrthoNear: -1
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1280
      ViewportHeight: 720
  - ID: 12758287598739165521
    Name: CameraController
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ClassName: HW2.CameraController
      ScriptFields:
        - Name: TrackMoveSpeed
          Type: Float
          Data: 0.300000012
        - Name: FpsMouseSensitivity
          Type: Float
          Data: 6
  - ID: 6196780381642991916
    Name: TrackBegin
    TransformComponent:
      Translation: [-12.9431667, 6.05935097, 21.578743]
      Rotation: [0, -0.289854109, 0]
      Scale: [0.999999583, 1, 0.999999583]
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
  - ID: 7435904767540507848
    Name: TrackEnd
    TransformComponent:
      Translation: [26.5408707, 1.92529178, -31.8772202]
      Rotation: [3.11949778, 0.983592391, 2.96134067]
      Scale: [0.999992192, 0.999997973, 0.999994457]
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
  - ID: 2512236796659933535
    Name: Sphere
    TransformComponent:
      Translation: [-10.5939713, 7.17551756, -2.75045586]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 800265512724681865
    Name: Firekeeper
    TransformComponent:
      Translation: [-8.28960514, -3.1949101e-05, -7.41834354]
      Rotation: [-3.14159226, -0.804629147, 3.14159226]
      Scale: [4, 4, 4]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Firekeeper/Firekeeper.fbx
      Materials:
        - MaterialName: shoes\N:\FDP\data\Model\chr\c1400\tex\c1400_leg_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_leg_a.png
        - MaterialName: arm\N:\FDP\data\Model\chr\c1400\tex\c1400_arm_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_arm_a.png
        - MaterialName: circlet\N:\FDP\data\Model\chr\c1400\tex\c1400_head_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_head_a.png
        - MaterialName: body\N:\FDP\data\Model\chr\c1400\tex\c1400_body_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_body_a.png
        - MaterialName: manteau\N:\FDP\data\Model\chr\c1400\tex\c1400_manteau_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_manteau_a.png
        - MaterialName: manteau_Cloth\N:\FDP\data\Model\chr\c1400\tex\c1400_manteau_a.t
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_manteau_a.png
        - MaterialName: skirt_e_Cloth\N:\FDP\data\Model\chr\c1400\tex\c1400_leg_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_leg_a.png
        - MaterialName: eye\N:\FDP\data\Model\chr\c1400\tex\c1400_skin_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_skin_a.png
        - MaterialName: hair\N:\FDP\data\Model\chr\c1400\tex\c1400_hair_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_hair_a.png
        - MaterialName: body_e\N:\FDP\data\Model\chr\c1400\tex\c1400_body_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_body_a.png
        - MaterialName: hair_e\N:\FDP\data\Model\chr\c1400\tex\c1400_hair_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_hair_a.png
        - MaterialName: skin\N:\FDP\data\Model\chr\c1400\tex\c1400_skin_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_skin_a.png
        - MaterialName: skirt_e\N:\FDP\data\Model\chr\c1400\tex\c1400_leg_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_leg_a.png
        - MaterialName: body_Cloth\N:\FDP\data\Model\chr\c1400\tex\c1400_body_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_body_a.png
        - MaterialName: body_e_Cloth\N:\FDP\data\Model\chr\c1400\tex\c1400_body_a.tif
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_body_a.png
      Bones:
        - Parent: "null"
          Child: manteau_01
        - Parent: "null"
          Child: arm
        - Parent: "null"
          Child: body
        - Parent: "null"
          Child: skirt
        - Parent: "null"
          Child: belt
        - Parent: "null"
          Child: necklace
        - Parent: "null"
          Child: eye
        - Parent: "null"
          Child: roop
        - Parent: "null"
          Child: hair
        - Parent: "null"
          Child: head
        - Parent: "null"
          Child: circlet
        - Parent: "null"
          Child: shoes
        - Parent: "null"
          Child: hand
        - Parent: "null"
          Child: leg
        - Parent: "null"
          Child: manteau_02
        - Parent: "null"
          Child: Master
        - Parent: Master
          Child: Throw_Target
        - Parent: Master
          Child: Pelvis
        - Parent: Pelvis
          Child: L_Thigh
        - Parent: L_Thigh
          Child: L_ThighTwist
        - Parent: L_Thigh
          Child: L_ThighTwist1
        - Parent: L_Thigh
          Child: L_Knee
        - Parent: L_Thigh
          Child: L_Calf
        - Parent: L_Calf
          Child: L_CalfTwist
        - Parent: L_Calf
          Child: L_CalfTwist1
        - Parent: L_Calf
          Child: L_Foot
        - Parent: L_Foot
          Child: L_Toe0
        - Parent: L_Calf
          Child: L_Calf_Skirt
        - Parent: L_Thigh
          Child: L_Thigh_Skirt
        - Parent: L_Thigh
          Child: L_Knee_Skirt
        - Parent: Pelvis
          Child: R_Thigh
        - Parent: R_Thigh
          Child: R_ThighTwist
        - Parent: R_Thigh
          Child: R_ThighTwist1
        - Parent: R_Thigh
          Child: R_Knee
        - Parent: R_Thigh
          Child: R_Calf
        - Parent: R_Calf
          Child: R_CalfTwist
        - Parent: R_Calf
          Child: R_CalfTwist1
        - Parent: R_Calf
          Child: R_Foot
        - Parent: R_Foot
          Child: R_Toe0
        - Parent: R_Calf
          Child: R_Calf_Skirt
        - Parent: R_Thigh
          Child: R_Thigh_Skirt
        - Parent: R_Thigh
          Child: R_Knee_Skirt
        - Parent: Master
          Child: Spine
        - Parent: Spine
          Child: Spine1
        - Parent: Spine1
          Child: Spine2
        - Parent: Spine2
          Child: L_Clavicle
        - Parent: L_Clavicle
          Child: L_Shoulder
        - Parent: L_Clavicle
          Child: L_UpperArm
        - Parent: L_UpperArm
          Child: L_UpArmTwist
        - Parent: L_UpperArm
          Child: L_UpArmTwist1
        - Parent: L_UpperArm
          Child: L_Elbow
        - Parent: L_UpperArm
          Child: L_Forearm
        - Parent: L_Forearm
          Child: L_Hand_
        - Parent: L_Hand_
          Child: L_Finger0
        - Parent: L_Finger0
          Child: L_Finger01
        - Parent: L_Finger01
          Child: L_Finger02
        - Parent: L_Hand_
          Child: L_Finger1
        - Parent: L_Finger1
          Child: L_Finger11
        - Parent: L_Finger11
          Child: L_Finger12
        - Parent: L_Hand_
          Child: L_Finger2
        - Parent: L_Finger2
          Child: L_Finger21
        - Parent: L_Finger21
          Child: L_Finger22
        - Parent: L_Hand_
          Child: L_Finger3
        - Parent: L_Finger3
          Child: L_Finger31
        - Parent: L_Finger31
          Child: L_Finger32
        - Parent: L_Hand_
          Child: L_Finger4
        - Parent: L_Finger4
          Child: L_Finger41
        - Parent: L_Finger41
          Child: L_Finger42
        - Parent: L_Forearm
          Child: L_ForeArmTwist
        - Parent: L_Forearm
          Child: L_ForeArmTwist1
        - Parent: Spine2
          Child: R_Clavicle
        - Parent: R_Clavicle
          Child: R_Shoulder
        - Parent: R_Clavicle
          Child: R_UpperArm
        - Parent: R_UpperArm
          Child: R_UpArmTwist
        - Parent: R_UpperArm
          Child: R_UpArmTwist1
        - Parent: R_UpperArm
          Child: R_Elbow
        - Parent: R_UpperArm
          Child: R_Forearm
        - Parent: R_Forearm
          Child: R_Hand
        - Parent: R_Hand
          Child: R_Finger0
        - Parent: R_Finger0
          Child: R_Finger01
        - Parent: R_Finger01
          Child: R_Finger02
        - Parent: R_Hand
          Child: R_Finger1
        - Parent: R_Finger1
          Child: R_Finger11
        - Parent: R_Finger11
          Child: R_Finger12
        - Parent: R_Hand
          Child: R_Finger2
        - Parent: R_Finger2
          Child: R_Finger21
        - Parent: R_Finger21
          Child: R_Finger22
        - Parent: R_Hand
          Child: R_Finger3
        - Parent: R_Finger3
          Child: R_Finger31
        - Parent: R_Finger31
          Child: R_Finger32
        - Parent: R_Hand
          Child: R_Finger4
        - Parent: R_Finger4
          Child: R_Finger41
        - Parent: R_Finger41
          Child: R_Finger42
        - Parent: R_Forearm
          Child: R_ForeArmTwist
        - Parent: R_Forearm
          Child: R_ForeArmTwist1
        - Parent: Spine2
          Child: Neck
        - Parent: Neck
          Child: Head
        - Parent: Head
          Child: Jaw
        - Parent: "null"
          Child: Dummy___
        - Parent: "null"
          Child: Dummy_h_SFX
        - Parent: "null"
          Child: Dummy___.001
        - Parent: "null"
          Child: body_arm
        - Parent: "null"
          Child: hand.001
        - Parent: "null"
          Child: R_Foot_Target2
        - Parent: "null"
          Child: R_Foot_Target1
        - Parent: "null"
          Child: R_Foot_Target
        - Parent: "null"
          Child: L_Foot_Target2
        - Parent: "null"
          Child: L_Foot_Target1
        - Parent: "null"
          Child: L_Foot_Target
        - Parent: "null"
          Child: Throw_Target2
        - Parent: "null"
          Child: Throw_Target1
        - Parent: "null"
          Child: Root
        - Parent: "null"
          Child: L_Weapon001
        - Parent: "null"
          Child: R_Weapon001