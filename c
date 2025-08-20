--[[
  This script should be placed inside a LocalScript, for example, in StarterPlayer > StarterPlayerScripts in Roblox.
  It will create a draggable GUI with a slider to control player speed when jumping and moving.
]]

-- Services
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")

-- Player
local player = game.Players.LocalPlayer

-- Configuration
local DEFAULT_WALKSPEED = 16 -- Default Roblox walkspeed
local MIN_SPEED = DEFAULT_WALKSPEED
local MAX_SPEED = 200 -- Maximum speed for the glitch
local currentSpeed = MIN_SPEED

-- Create GUI
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "SpeedGlitchGUI"
screenGui.Parent = player:WaitForChild("PlayerGui")
screenGui.ResetOnSpawn = false

local mainFrame = Instance.new("Frame")
mainFrame.Name = "MainFrame"
mainFrame.Size = UDim2.new(0, 250, 0, 150)
mainFrame.Position = UDim2.new(0.5, -125, 0.5, -75)
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

local speedLabel = Instance.new("TextLabel")
speedLabel.Name = "SpeedLabel"
speedLabel.Size = UDim2.new(1, 0, 0, 20)
speedLabel.Position = UDim2.new(0, 0, 0, 85)
speedLabel.BackgroundColor3 = Color3.new(1, 1, 1)
speedLabel.BackgroundTransparency = 1
speedLabel.TextColor3 = Color3.fromRGB(255, 255, 255)
speedLabel.Text = "Speed: " .. tostring(math.floor(currentSpeed))
speedLabel.Font = Enum.Font.SourceSans
speedLabel.TextSize = 16
speedLabel.Parent = mainFrame

local toggleButton = Instance.new("TextButton")
toggleButton.Name = "ToggleButton"
toggleButton.Size = UDim2.new(0.8, 0, 0, 25)
toggleButton.Position = UDim2.new(0.1, 0, 0, 115)
toggleButton.BackgroundColor3 = Color3.fromRGB(80, 180, 80)
toggleButton.TextColor3 = Color3.fromRGB(255, 255, 255)
toggleButton.Text = "Speed Glitch: ON"
toggleButton.Font = Enum.Font.SourceSansBold
toggleButton.TextSize = 16
toggleButton.Parent = mainFrame

-- GUI Logic & State
local isGlitchEnabled = true

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

-- Slider Logic for PC and Mobile
local dragging = false

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
        
        currentSpeed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * percentage
        speedLabel.Text = "Speed: " .. tostring(math.floor(currentSpeed))
    end
end)

-- Character Handling & Core Logic
local runServiceConnection = nil

local function onCharacterAdded(character)
    local humanoid = character:WaitForChild("Humanoid")

    -- Disconnect previous connection to prevent memory leaks
    if runServiceConnection then
        runServiceConnection:Disconnect()
        runServiceConnection = nil
    end

    -- Set WalkSpeed to default when the script starts or character respawns
    humanoid.WalkSpeed = DEFAULT_WALKSPEED

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
            if humanoid.WalkSpeed ~= currentSpeed then
                humanoid.WalkSpeed = currentSpeed
            end
        else
            if humanoid.WalkSpeed ~= DEFAULT_WALKSPEED then
                humanoid.WalkSpeed = DEFAULT_WALKSPEED
            end
        end
    end)
end

-- Connect to future character spawns
player.CharacterAdded:Connect(onCharacterAdded)

-- Handle character if it already exists when the script runs
if player.Character then
    onCharacterAdded(player.Character)
end
