const frontSlider = document.getElementById("frontSlider");
const backSlider = document.getElementById("backSlider");

const frontStatus = document.getElementById("frontStatus");
const backStatus = document.getElementById("backStatus");


frontSlider.addEventListener("input", () => {
    if (Number(frontSlider.value) === 1) {
        frontStatus.textContent = "Status: Front is moving";
        alert("Front Door detected");
    } else {
        frontStatus.textContent = "Status: Idle";
    }
});

backSlider.addEventListener("input", () => {
    if (Number(backSlider.value) === 1) {
        backStatus.textContent = "Status: Back is moving";
        alert("Back Door detected");
    } else {
        backStatus.textContent = "Status: Idle";
    }
});