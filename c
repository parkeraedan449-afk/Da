--[[
  LocalScript (e.g., StarterPlayer > StarterPlayerScripts)
  Creates a draggable GUI with slider + text input for speed,
  and text input for acceleration (safe capped).
]]

-- Services
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")

-- Player
local player = game.Players.LocalPlayer

-- Configuration
local DEFAULT_WALKSPEED = 16
local MIN_SPEED = DEFAULT_WALKSPEED
local MAX_SPEED = 20000
local currentSpeed = MIN_SPEED
local accelerationRate = 0.05 -- can now go up to 5 safely

-- Create GUI
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "SpeedGlitchGUI"
screenGui.Parent = player:WaitForChild("PlayerGui")
screenGui.ResetOnSpawn = false

local mainFrame = Instance.new("Frame")
mainFrame.Name = "MainFrame"
mainFrame.Size = UDim2.new(0, 250, 0, 200)
mainFrame.Position = UDim2.new(0.5, -125, 0.5, -100)
mainFrame.BackgroundColor3 = Color3.fromRGB(35, 35, 35)
mainFrame.BorderColor3 = Color3.fromRGB(80, 80, 80)
mainFrame.BorderSizePixel = 2
mainFrame.Active = true
mainFrame.Draggable = true
mainFrame.Parent = screenGui

local titleLabel = Instance.new("TextLabel")
titleLabel.Name = "TitleLabel"
titleLabel.Size = UDim2.new(1, 0, 0, 30)
titleLabel.BackgroundColor3 = Color3.fromRGB(50, 50, 50)
titleLabel.TextColor3 = Color3.fromRGB(255, 255, 255)
titleLabel.Text = "Speed Glitch"
titleLabel.Font = Enum.Font.SourceSansBold
titleLabel.TextSize = 18
titleLabel.Parent = mainFrame

local closeButton = Instance.new("TextButton")
closeButton.Name = "CloseButton"
closeButton.Size = UDim2.new(0, 30, 0, 30)
closeButton.Position = UDim2.new(1, -30, 0, 0)
closeButton.BackgroundColor3 = Color3.fromRGB(200, 50, 50)
closeButton.TextColor3 = Color3.fromRGB(255, 255, 255)
closeButton.Text = "X"
closeButton.Font = Enum.Font.SourceSansBold
closeButton.TextSize = 18
closeButton.Parent = titleLabel

-- Speed slider
local speedSlider = Instance.new("Frame")
speedSlider.Name = "SpeedSlider"
speedSlider.Size = UDim2.new(0.8, 0, 0, 20)
speedSlider.Position = UDim2.new(0.1, 0, 0, 50)
speedSlider.BackgroundColor3 = Color3.fromRGB(60, 60, 60)
speedSlider.Parent = mainFrame

local sliderHandle = Instance.new("TextButton")
sliderHandle.Name = "SliderHandle"
sliderHandle.Size = UDim2.new(0, 10, 0, 30)
sliderHandle.Position = UDim2.new(0, -5, -0.25, 0)
sliderHandle.BackgroundColor3 = Color3.fromRGB(100, 180, 255)
sliderHandle.BorderSizePixel = 0
sliderHandle.Parent = speedSlider

-- Speed label + text box
local speedLabel = Instance.new("TextLabel")
speedLabel.Name = "SpeedLabel"
speedLabel.Size = UDim2.new(0.4, 0, 0, 20)
speedLabel.Position = UDim2.new(0.1, 0, 0, 85)
speedLabel.BackgroundColor3 = Color3.fromRGB(50, 50, 50)
speedLabel.TextColor3 = Color3.fromRGB(255, 255, 255)
speedLabel.Text = "Speed:"
speedLabel.Font = Enum.Font.SourceSans
speedLabel.TextSize = 16
speedLabel.TextXAlignment = Enum.TextXAlignment.Left
speedLabel.Parent = mainFrame

local speedTextBox = Instance.new("TextBox")
speedTextBox.Name = "SpeedTextBox"
speedTextBox.Size = UDim2.new(0.4, 0, 0, 25)
speedTextBox.Position = UDim2.new(0.55, 0, 0, 80)
speedTextBox.BackgroundColor3 = Color3.fromRGB(70, 70, 70)
speedTextBox.TextColor3 = Color3.fromRGB(255, 255, 255)
speedTextBox.Text = tostring(currentSpeed)
speedTextBox.Font = Enum.Font.SourceSans
speedTextBox.TextSize = 16
speedTextBox.ClearTextOnFocus = false
speedTextBox.Parent = mainFrame

-- Acceleration
local accelLabel = Instance.new("TextLabel")
accelLabel.Name = "AccelLabel"
accelLabel.Size = UDim2.new(0.4, 0, 0, 20)
accelLabel.Position = UDim2.new(0.1, 0, 0, 115)
accelLabel.BackgroundColor3 = Color3.fromRGB(50, 50, 50)
accelLabel.TextColor3 = Color3.fromRGB(255, 255, 255)
accelLabel.Text = "Acceleration:"
accelLabel.Font = Enum.Font.SourceSans
accelLabel.TextSize = 16
accelLabel.TextXAlignment = Enum.TextXAlignment.Left
accelLabel.Parent = mainFrame

local accelTextBox = Instance.new("TextBox")
accelTextBox.Name = "AccelTextBox"
accelTextBox.Size = UDim2.new(0.4, 0, 0, 25)
accelTextBox.Position = UDim2.new(0.55, 0, 0, 110)
accelTextBox.BackgroundColor3 = Color3.fromRGB(70, 70, 70)
accelTextBox.TextColor3 = Color3.fromRGB(255, 255, 255)
accelTextBox.Text = tostring(accelerationRate)
accelTextBox.Font = Enum.Font.SourceSans
accelTextBox.TextSize = 16
accelTextBox.ClearTextOnFocus = false
accelTextBox.Parent = mainFrame

-- Toggle button
local toggleButton = Instance.new("TextButton")
toggleButton.Name = "ToggleButton"
toggleButton.Size = UDim2.new(0.8, 0, 0, 25)
toggleButton.Position = UDim2.new(0.1, 0, 0, 150)
toggleButton.BackgroundColor3 = Color3.fromRGB(80, 180, 80)
toggleButton.TextColor3 = Color3.fromRGB(255, 255, 255)
toggleButton.Text = "Speed Glitch: ON"
toggleButton.Font = Enum.Font.SourceSansBold
toggleButton.TextSize = 16
toggleButton.Parent = mainFrame

-- GUI Logic
local isGlitchEnabled = true
local dragging = false

closeButton.MouseButton1Click:Connect(function()
    screenGui:Destroy()
end)

toggleButton.MouseButton1Click:Connect(function()
    isGlitchEnabled = not isGlitchEnabled
    if isGlitchEnabled then
        toggleButton.Text = "Speed Glitch: ON"
        toggleButton.BackgroundColor3 = Color3.fromRGB(80, 180, 80)
    else
        toggleButton.Text = "Speed Glitch: OFF"
        toggleButton.BackgroundColor3 = Color3.fromRGB(180, 80, 80)
    end
end)

-- Slider control
sliderHandle.InputBegan:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
        dragging = true
    end
end)

UserInputService.InputEnded:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
        dragging = false
    end
end)

UserInputService.InputChanged:Connect(function(input)
    if dragging and (input.UserInputType == Enum.UserInputType.MouseMovement or input.UserInputType == Enum.UserInputType.Touch) then
        local sliderWidth = speedSlider.AbsoluteSize.X
        local sliderX = speedSlider.AbsolutePosition.X

        local relativeX = math.clamp(input.Position.X - sliderX, 0, sliderWidth)
        local percentage = relativeX / sliderWidth

        sliderHandle.Position = UDim2.new(percentage, -sliderHandle.AbsoluteSize.X / 2, -0.25, 0)

        -- exponential scaling
        currentSpeed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * (percentage ^ 4)
        speedTextBox.Text = tostring(math.floor(currentSpeed)) -- sync with input
    end
end)

-- Speed box logic
speedTextBox.FocusLost:Connect(function(enterPressed)
    if enterPressed then
        local inputValue = tonumber(speedTextBox.Text)
        if inputValue and inputValue >= MIN_SPEED and inputValue <= MAX_SPEED then
            currentSpeed = inputValue
            speedTextBox.TextColor3 = Color3.fromRGB(255, 255, 255)

            -- update slider position to match input
            local percentage = ((currentSpeed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED)) ^ (1/4)
            sliderHandle.Position = UDim2.new(percentage, -sliderHandle.AbsoluteSize.X / 2, -0.25, 0)
        else
            speedTextBox.Text = tostring(math.floor(currentSpeed))
            speedTextBox.TextColor3 = Color3.fromRGB(255, 100, 100)
        end
    end
end)

-- Acceleration box logic (safe capped)
accelTextBox.FocusLost:Connect(function(enterPressed)
    if enterPressed then
        local inputValue = tonumber(accelTextBox.Text)
        if inputValue and inputValue > 0 and inputValue <= 5 then -- safe cap at 5
            accelerationRate = inputValue
            accelTextBox.TextColor3 = Color3.fromRGB(255, 255, 255)
        else
            accelTextBox.Text = tostring(accelerationRate)
            accelTextBox.TextColor3 = Color3.fromRGB(255, 100, 100)
        end
    end
end)

-- Core Logic
local runServiceConnection = nil

local function onCharacterAdded(character)
    local humanoid = character:WaitForChild("Humanoid")
    humanoid.WalkSpeed = DEFAULT_WALKSPEED

    if runServiceConnection then
        runServiceConnection:Disconnect()
    end

    runServiceConnection = RunService.RenderStepped:Connect(function()
        if not humanoid or humanoid:GetState() == Enum.HumanoidStateType.Dead then return end

        if not isGlitchEnabled then
            if humanoid.WalkSpeed ~= DEFAULT_WALKSPEED then
                humanoid.WalkSpeed = DEFAULT_WALKSPEED
            end
            return
        end

        local state = humanoid:GetState()
        local isJumping = state == Enum.HumanoidStateType.Jumping or state == Enum.HumanoidStateType.Freefall
        local isMoving = humanoid.MoveDirection.Magnitude > 0.1

        if isJumping and isMoving then
            -- acceleration with safe cap
            local diff = currentSpeed - humanoid.WalkSpeed
            humanoid.WalkSpeed = humanoid.WalkSpeed + diff * math.clamp(accelerationRate, 0, 5)
        else
            if humanoid.WalkSpeed ~= DEFAULT_WALKSPEED then
                humanoid.WalkSpeed = DEFAULT_WALKSPEED
            end
        end
    end)
end

player.CharacterAdded:Connect(onCharacterAdded)
if player.Character then
    onCharacterAdded(player.Character)
end
