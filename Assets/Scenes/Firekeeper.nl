Scene: Untitled
Entities:
  - ID: 800265512724681865
    Name: Firekeeper
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [3, 3, 3]
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