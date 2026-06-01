export module Example.CircularWave3DLayer;

import Nodens.Events;
import Nodens.Layer;
import Nodens.TimeStep;
import std;

export class CircularWave3DLayer : public Nodens::Layer
{
public:
    CircularWave3DLayer();

    void OnUpdate(Nodens::TimeStep ts) override;
    void OnImGuiRender(Nodens::TimeStep ts) override;
    void OnEvent(Nodens::Event& event) override;

private:
    static constexpr std::size_t kNumberOfPoints = 1000;

    float kFrequency = 5.f * std::numbers::pi_v<float>;
    float t[kNumberOfPoints], xs[kNumberOfPoints], ys[kNumberOfPoints];
};
