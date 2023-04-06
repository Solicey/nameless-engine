Scene: Untitled
Entities:
  - ID: 6481490052938585061
    Name: Man
    TransformComponent:
      Translation: [0.18769908, 0, 2.27937889]
      Rotation: [3.14159203, 0.973827064, -3.14159226]
      Scale: [0.999999881, 1, 0.999999881]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/nanosuit/nanosuit.obj
      Materials:
        - MaterialName: Glass
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/glass_dif.png
        - MaterialName: Leg
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/leg_dif.png
        - MaterialName: Helmet
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/helmet_diff.png
        - MaterialName: Hand
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/hand_dif.png
        - MaterialName: Body
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/body_dif.png
        - MaterialName: Arm
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets/Models/nanosuit/arm_dif.png
      Bones:
        []
  - ID: 6795186737215464914
    Name: Sphere
    TransformComponent:
      Translation: [4.71550846, 11.1985302, 10.8842192]
      Rotation: [-3.14159226, -0.681466162, -3.14159226]
      Scale: [8.40039539, 1, 1.00003028]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 15863501735684059158
    Name: Box
    TransformComponent:
      Translation: [-9.73278141, 1.09730661, 0]
      Rotation: [-1.12301576, 0.544543386, 0.111701638]
      Scale: [1, 0.999999881, 0.999998868]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 9426063211989296428
    Name: Camera
    TransformComponent:
      Translation: [0.193741545, 7.35947371, -13.2488604]
      Rotation: [2.51547718, 0.803646028, 2.78376532]
      Scale: [0.999999881, 1, 1.00000012]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Camera.obj
      Materials:
        - MaterialName: (null)
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []