#include "Fade.h"
#include"algorithm"
using namespace KamataEngine;
Fade::Fade() {}

Fade::~Fade() {
	delete sprite_;
	sprite_ = nullptr;
}
void Fade::Initialize() {
	textureHandle_ = TextureManager::Load("white1x1.png"); // Load the fade texture
	sprite_ = Sprite::Create(textureHandle_, {1, 1});      // Create a new sprite for the fade effect

	sprite_->SetSize(Vector2{1280, 720});   // Set sprite size to match the screen resolution
	sprite_->SetColor(Vector4{0, 0, 0, 0}); // Set initial color to white (opaque)
}
void Fade::Update() {  
// Update logic for fade effect  

switch (status_) {  
case Status::None:  
	// No fade effect  
	break;  
case Status::FadeIn:  
	// Fade in effect  
	counter_ += 1.0f / 60; // Increment counter based on duration  
	if (counter_ >= duration_) {  
		counter_ = duration_; // Clamp counter to 1.0  
	}  
	sprite_->SetColor(Vector4{0, 0, 0, (float)(1.0f - (std::clamp(counter_ / duration_, 0.0f, 1.0f)))}); // Update sprite color for fade in  
	break; // Add break to prevent fallthrough  
case Status::FadeOut:  
	// Fade out effect  
	counter_ += 1.0f / 60; // Increment counter based on duration  
	if (counter_ >= duration_) {  
		counter_ = duration_; // Clamp counter to 1.0  
	}  
	sprite_->SetColor(Vector4{0, 0, 0, (float)(std::clamp(counter_ / duration_, 0.0f, 1.0f))}); // Update sprite color for fade out  
	break;  
}  
}
void Fade::Draw() {
	// Draw logic for fade effect
	Sprite::PreDraw();
	sprite_->Draw();
	if (status_ == Status::None) {
		return; // If no fade effect, skip drawing
	}
	Sprite::PostDraw();
}

void Fade::start(Status status, float duration) {

	status_ = status;     // Set the fade status
	duration_ = duration; // Set the duration for the fade effect
	counter_ = 0.0f;      // Reset the counter to start the fade effect

}

void Fade::stop() {
	status_ = Status::None; // Stop the fade effect by setting status to None

}

bool Fade::isFinished() const { 
switch (status_) {
case Status::FadeIn:
	
case Status::FadeOut:
	if (counter_ >= duration_) {
		return true; // Fade-out is complete
	} else {
		return false; // Fade-out is still in progress
	}

}
return true;
}
