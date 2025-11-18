var Module = {
  print: (function () {
    return (text) => {
      console.log(text);
    };
  })(),
  canvas: (function () {
    return document.getElementById("canvas");
  })(),
  locateFile: function (url) {
    return "scripts/" + url;
  },
};

initWebGPU = async () => {
  // Check to ensure the user agent supports WebGPU
  if (!("gpu" in navigator)) {
    const msg = "⚠️ WebGPU is not available on this browser.";

    const pre = document.createElement("pre");
    pre.style.color = "#f00";
    pre.style.textAlign = "center";
    pre.textContent = msg;
    document.body.appendChild(pre);

    console.error(msg);

    return false;
  }

  // Request an adapter
  const adapter = await navigator.gpu.requestAdapter();
  if (!adapter) {
    console.error("No WebGPU adapters found.");
    return false;
  }

  // Request a device
  const device = await adapter.requestDevice();
  device.lost.then((info) => {
    console.error(`WebGPU device was lost: ${info.message}`);
    device = null;

    if (info.reason != "destroyed") {
      initWebGPU();
    }
  });

  // Set WebGPU device in Module
  Module.preinitializedWebGPUDevice = device;

  return true;
};

initWebGPU();
